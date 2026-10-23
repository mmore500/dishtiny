#!/usr/bin/env python3

__doc__ = """Stitches the png frames in a web interface download into videos.

Each viewer's frames are in a folder named for the viewer, with a subfolder
for each series index if the viewer has multiple series. One video is made per
folder of frames, ordered by update.

Usage:
    ./stitch_saved_frames.py <saved folder or zip> <output folder> [fps]

Videos are named after their folder, e.g., `kin-group-id.mp4` or
`expression-by-module+series=3.mp4`.
"""

import cv2
import os
import pathlib
import re
import sys
import tempfile
import zipfile


def get_update(path):
    # keyname attributes are separated by '+', e.g., title=alive+update=10
    match = re.search(r"(?:^|\+)update=(\d+)", path.name)
    return int(match.group(1)) if match else -1


def get_video_name(root, folder):
    parts = folder.relative_to(root).parts
    name = parts[0]
    if len(parts) > 1:
        name += "+series=" + "+".join(parts[1:])
    return name + ".mp4"


def make_video(frames, out_path, fps):
    height, width, _ = cv2.imread(str(frames[0])).shape
    video = cv2.VideoWriter(
        str(out_path), cv2.VideoWriter_fourcc(*"mp4v"), fps, (width, height)
    )
    for frame_path in frames:
        frame = cv2.imread(str(frame_path))
        if frame.shape[:2] != (height, width):
            frame = cv2.resize(frame, (width, height))
        video.write(frame)
    video.release()


def stitch(root, out_dir, fps):
    out_dir.mkdir(parents=True, exist_ok=True)
    folders = sorted({png.parent for png in root.rglob("*.png")})
    for folder in folders:
        frames = sorted(folder.glob("*.png"), key=get_update)
        out_path = out_dir / get_video_name(root, folder)
        print("stitching", len(frames), "frames into", out_path)
        make_video(frames, out_path, fps)


if __name__ == "__main__":

    if len(sys.argv) not in {3, 4}:
        print(__doc__)
        sys.exit(1)

    source = pathlib.Path(sys.argv[1])
    out_dir = pathlib.Path(sys.argv[2])
    fps = int(sys.argv[3]) if len(sys.argv) == 4 else 10

    if zipfile.is_zipfile(source):
        with tempfile.TemporaryDirectory() as tmp:
            with zipfile.ZipFile(source) as archive:
                archive.extractall(tmp)
            stitch(pathlib.Path(tmp), out_dir, fps)
    else:
        stitch(source, out_dir, fps)
