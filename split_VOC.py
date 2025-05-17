import os
import xml.etree.ElementTree as ET
import shutil
import random
from collections import defaultdict
from tqdm import tqdm

# --------------------------
# ✅ 可修改的参数区
# --------------------------
VOC_ROOT = '../datasets/VOCdevkit'   # VOC2007 和 VOC2012 的根目录
OUTPUT_ROOT = '../datasets'          # 结果输出目录
SHOT = 5                          # 每个 novel 类的 few-shot 数量
SPLITS_TO_GENERATE = [1, 2, 3]    # 生成的 split 编号（1、2、3）
# --------------------------

VOC_CLASSES = [
    'aeroplane', 'bicycle', 'bird', 'boat', 'bottle',
    'bus', 'car', 'cat', 'chair', 'cow',
    'diningtable', 'dog', 'horse', 'motorbike', 'person',
    'pottedplant', 'sheep', 'sofa', 'train', 'tvmonitor'
]

SPLITS = {
    1: ['pottedplant', 'sheep', 'sofa', 'train', 'tvmonitor'],
    2: ['aeroplane', 'sheep', 'sofa', 'train', 'tvmonitor'],
    3: ['bicycle', 'chair', 'sofa', 'train', 'tvmonitor']
}

def convert_bbox(size, box):
    dw, dh = 1. / size[0], 1. / size[1]
    x = (box[0] + box[1]) / 2.0
    y = (box[2] + box[3]) / 2.0
    w = box[1] - box[0]
    h = box[3] - box[2]
    return x * dw, y * dh, w * dw, h * dh

def process_file(img_id, year, voc_root, class_to_idx, base_classes, novel_classes,
                 novel_limit, novel_count, output_root, split_tag):
    xml_file = os.path.join(voc_root, f'VOC{year}/Annotations/{img_id}.xml')
    img_file = os.path.join(voc_root, f'VOC{year}/JPEGImages/{img_id}.jpg')
    tree = ET.parse(xml_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    yolo_lines = []
    classes_in_image = set()

    for obj in root.findall('object'):
        cls = obj.find('name').text
        if cls not in class_to_idx:
            continue
        bbox = obj.find('bndbox')
        box = (
            float(bbox.find('xmin').text),
            float(bbox.find('xmax').text),
            float(bbox.find('ymin').text),
            float(bbox.find('ymax').text)
        )
        yolo_box = convert_bbox((w, h), box)
        cls_id = class_to_idx[cls]
        yolo_lines.append(f"{cls_id} {' '.join([str(round(x, 6)) for x in yolo_box])}")
        classes_in_image.add(cls)

    if not yolo_lines:
        return

    if split_tag == 'train':
        if all(cls in base_classes for cls in classes_in_image):
            target = 'train_base'
        elif all(cls in novel_classes for cls in classes_in_image):
            if any(novel_count[c] >= novel_limit[c] for c in classes_in_image):
                return
            for c in classes_in_image:
                novel_count[c] += 1
            target = 'train_novel'
        else:
            return
    else:
        target = split_tag  # val or test

    out_img = os.path.join(output_root, 'images', target, f'{img_id}.jpg')
    out_label = os.path.join(output_root, 'labels', target, f'{img_id}.txt')
    shutil.copyfile(img_file, out_img)
    with open(out_label, 'w') as f:
        f.write('\n'.join(yolo_lines))

def voc_to_yolo(split_id):
    novel_classes = SPLITS[split_id]
    base_classes = [c for c in VOC_CLASSES if c not in novel_classes]
    class_to_idx = {c: i for i, c in enumerate(VOC_CLASSES)}
    novel_count = defaultdict(int)
    novel_limit = {cls: SHOT for cls in novel_classes}

    output_root = os.path.join(OUTPUT_ROOT, f'VOC_split{split_id}_yolo')
    for subset in ['train_base', 'train_novel', 'test', 'val']:
        os.makedirs(os.path.join(output_root, 'images', subset), exist_ok=True)
        os.makedirs(os.path.join(output_root, 'labels', subset), exist_ok=True)

    print(f"\n🔧 生成 Split {split_id}（Base 类: {len(base_classes)}，Novel 类: {len(novel_classes)}）")

    for year in ['2007', '2012']:
        split_file = os.path.join(VOC_ROOT, f'VOC{year}/ImageSets/Main/trainval.txt')
        with open(split_file) as f:
            img_ids = [line.strip() for line in f.readlines()]
        for img_id in tqdm(img_ids, desc=f"Trainval {year}"):
            process_file(img_id, year, VOC_ROOT, class_to_idx, base_classes,
                         novel_classes, novel_limit, novel_count, output_root, 'train')

    split_file = os.path.join(VOC_ROOT, 'VOC2007/ImageSets/Main/test.txt')
    with open(split_file) as f:
        img_ids = [line.strip() for line in f.readlines()]
    for img_id in tqdm(img_ids, desc="Val"):
        process_file(img_id, '2007', VOC_ROOT, class_to_idx, base_classes,
                     novel_classes, novel_limit, novel_count, output_root, 'val')
    for img_id in tqdm(img_ids, desc="Test"):
        process_file(img_id, '2007', VOC_ROOT, class_to_idx, base_classes,
                     novel_classes, novel_limit, novel_count, output_root, 'test')

    yaml_path = os.path.join(output_root, f'data_split{split_id}.yaml')
    with open(yaml_path, 'w') as f:
        f.write(f"path: {output_root}\n")
        f.write("train: images/train_base\n")
        f.write("val: images/val\n")
        f.write("test: images/test\n")
        f.write("names:\n")
        for i, cls in enumerate(VOC_CLASSES):
            f.write(f"  {i}: {cls}\n")

    print(f"✅ Split {split_id} 完成，YAML 写入：{yaml_path}")

if __name__ == "__main__":
    for split_id in SPLITS_TO_GENERATE:
        voc_to_yolo(split_id)
