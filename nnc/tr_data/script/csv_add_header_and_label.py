#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CSVの先頭にヘッダー x__0,x__1,x__2,label_id を挿入し、
全データ行の末尾にコマンドライン指定のラベル文字列を付与します。
- Python 3.10.5
- Windows / macOS / Linux 対応
"""

from __future__ import annotations
import argparse
import csv
from pathlib import Path
import sys

DEFAULT_HEADER = ["x__0", "x__1", "x__2", "label_id"]

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="CSV にヘッダーを追加し、各行の末尾にラベルを付けるツール"
    )
    p.add_argument("-i", "--input", required=True,
                   help="入力CSVファイルのパス")
    p.add_argument("-o", "--output", required=True,
                   help="出力CSVファイルのパス（任意の場所に保存可）")
    # 問題文の -label に合わせてエイリアスを付ける
    p.add_argument("-l", "--label", "-label", required=True,
                   help="各行の末尾に追加する文字列（例: 4 や back など）")
    p.add_argument("--encoding", default="utf-8",
                   help="入出力の文字コード（既定: utf-8）")
    p.add_argument("--delimiter", default=",",
                   help="CSVの区切り文字（既定: ,）")
    p.add_argument("--skip-empty", action="store_true",
                   help="空行をスキップする（既定: オフ）")
    return p.parse_args()

def main() -> int:
    args = parse_args()

    in_path = Path(args.input)
    out_path = Path(args.output)

    if in_path.resolve() == out_path.resolve():
        print("[ERROR] 入力と出力に同じパスは指定できません。", file=sys.stderr)
        return 2

    # 読み込み：universal newline / UTF-8 デフォルト
    with open(in_path, "r", encoding=args.encoding, newline="") as fin, \
         open(out_path, "w", encoding=args.encoding, newline="") as fout:

        reader = csv.reader(fin, delimiter=args.delimiter, skipinitialspace=True)
        writer = csv.writer(fout, delimiter=args.delimiter, lineterminator="\n")

        # 先頭にヘッダーを書き込む
        writer.writerow(DEFAULT_HEADER)

        for row in reader:
            # 空行処理
            if len(row) == 0 or (len(row) == 1 and row[0].strip() == ""):
                if args.skip-empty:
                    continue
                else:
                    # 空行もラベル付きで出したくない想定が多いのでスキップ推奨
                    # ただし要求にないため、デフォルトはスキップせず無視（出力しない）
                    continue

            # 行末にラベルを追加
            row_out = list(row) + [args.label]
            writer.writerow(row_out)

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
