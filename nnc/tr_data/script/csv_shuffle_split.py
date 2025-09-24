#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
1つのCSVをシャッフルし、指定割合でトレーニング/評価に分割するツール。
- 入力CSVの1行目はヘッダとして両方にコピー
- シャッフル対象は2行目以降（データ部）
- Python 3.10.5 / Windows, macOS, Linux
- 標準ライブラリのみ
"""

from __future__ import annotations
import argparse
import csv
import math
import random
import sys
from pathlib import Path
from typing import List

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="CSVをシャッフルしてトレーニング/評価に分割（最初の行はヘッダとして両方にコピー）"
    )
    p.add_argument("-i", "--input", required=True, help="入力CSVファイルのパス")
    p.add_argument("-tp", "--train-percent", type=int, required=True,
                   help="トレーニングへの分割割合（1〜99 の整数）")
    p.add_argument("-ot", "--out-train", required=True, help="出力トレーニングCSVのパス")
    p.add_argument("-ov", "--out-val", required=True, help="出力評価CSVのパス")

    # 実運用で便利な追加オプション
    p.add_argument("--seed", type=int, default=None, help="乱数シード（再現性が必要な場合に指定）")
    p.add_argument("--encoding", default="utf-8", help="入出力エンコーディング（既定: utf-8）")
    p.add_argument("--delimiter", default=",", help="CSV区切り文字（既定: ,）")
    p.add_argument("--newline", choices=["native", "unix", "windows"], default="native",
                   help="出力の改行コード（native: OS準拠, unix: \\n, windows: \\r\\n）")
    p.add_argument("--bom", choices=["on", "off"], default="off",
                   help="出力の先頭にUTF-8 BOMを付与（Excel向け）。既定: off")
    return p.parse_args()

def open_reader(path: Path, encoding: str, delimiter: str):
    f = open(path, "r", encoding=encoding, newline="")  # universal newline
    reader = csv.reader(f, delimiter=delimiter, skipinitialspace=False)
    return reader, f

def open_writer(path: Path, encoding: str, delimiter: str, newline_mode: str, bom: bool):
    newline_map = {"native": "", "unix": "\n", "windows": "\r\n"}
    nl = newline_map[newline_mode]
    path.parent.mkdir(parents=True, exist_ok=True)
    f = open(path, "w", encoding=encoding, newline=nl)
    if bom:
        f.write("\ufeff")
    writer = csv.writer(f, delimiter=delimiter, lineterminator="\n")
    return writer, f

def main() -> int:
    args = parse_args()

    if not (1 <= args.train_percent <= 99):
        print("[ERROR] -tp/--train-percent は 1〜99 の整数で指定してください。", file=sys.stderr)
        return 2

    in_path = Path(args.input)
    out_train = Path(args.out_train)
    out_val = Path(args.out_val)

    # 入出力の同一パスを軽く防止
    for p in (out_train, out_val):
        try:
            if in_path.resolve() == p.resolve():
                print(f"[ERROR] 入力と出力が同一パスです: {p}", file=sys.stderr)
                return 2
        except Exception:
            pass

    # 読み込み
    reader, fin = open_reader(in_path, args.encoding, args.delimiter)
    try:
        try:
            header = next(reader)
        except StopIteration:
            # 空ファイル
            header = []

        data_rows: List[List[str]] = [row for row in reader]
    finally:
        fin.close()

    # シャッフル
    rnd = random.Random(args.seed)
    rnd.shuffle(data_rows)

    # 分割
    n = len(data_rows)
    n_train = math.floor(n * args.train_percent / 100.0)
    n_val = n - n_train
    train_rows = data_rows[:n_train]
    val_rows = data_rows[n_train:]

    # 書き出し
    writer_t, ft = open_writer(out_train, args.encoding, args.delimiter, args.newline, args.bom == "on")
    writer_v, fv = open_writer(out_val, args.encoding, args.delimiter, args.newline, args.bom == "on")
    try:
        # ヘッダは両方にコピー
        writer_t.writerow(header)
        writer_v.writerow(header)

        # データ
        for r in train_rows:
            writer_t.writerow(r)
        for r in val_rows:
            writer_v.writerow(r)
    finally:
        ft.close()
        fv.close()

    # 簡単な統計をstderrに
    print(f"[INFO] total={n}, train={n_train} ({args.train_percent}%), val={n_val} ({100-args.train_percent}%)", file=sys.stderr)
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
