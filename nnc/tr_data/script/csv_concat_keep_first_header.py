#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CSVを結合し、出力の1行目は最初の入力CSVのヘッダ行のみを使用。
2つ目以降の入力CSVのヘッダ行はスキップしてデータ行のみ結合する。

- Python 3.10.5
- Windows / macOS / Linux 対応
- 標準ライブラリのみ使用
"""

from __future__ import annotations
import argparse
import csv
import sys
from pathlib import Path
from typing import List, Optional

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="CSV連結ツール（最初のヘッダのみを採用）"
    )
    p.add_argument(
        "-i", "--input", action="append", required=True,
        help="入力CSVファイル（複数指定可。先頭のファイルの1行目をヘッダとして採用）"
    )
    p.add_argument(
        "-o", "--output", required=True,
        help="出力CSVファイルのパス（任意の場所に保存可）"
    )
    p.add_argument(
        "--delimiter", default=",",
        help="CSV区切り文字（既定: ,）"
    )
    p.add_argument(
        "--encoding", default="utf-8",
        help="入出力の文字コード（既定: utf-8）"
    )
    p.add_argument(
        "--newline", choices=["native", "unix", "windows"], default="native",
        help="出力改行コード（native: OS準拠, unix: \\n, windows: \\r\\n）"
    )
    p.add_argument(
        "--bom", choices=["on", "off"], default="off",
        help="出力の先頭にUTF-8 BOMを付与（Excel向け）。既定: off"
    )
    p.add_argument(
        "--warn-header-mismatch", action="store_true",
        help="2つ目以降のファイルのヘッダが最初のヘッダと列数不一致の場合に警告を表示"
    )
    return p.parse_args()

def open_reader(path: Path, encoding: str, delimiter: str) -> csv.reader:
    f = open(path, "r", encoding=encoding, newline="")
    return csv.reader(f, delimiter=delimiter, skipinitialspace=False), f

def open_writer(path: Path, encoding: str, delimiter: str, newline_mode: str, bom: bool) -> tuple[csv.writer, any]:
    newline_map = {"native": "", "unix": "\n", "windows": "\r\n"}
    nl = newline_map[newline_mode]
    # 保存先ディレクトリが無ければ作る
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    f = open(path, "w", encoding=encoding, newline=nl)
    if bom:
        f.write("\ufeff")
    writer = csv.writer(f, delimiter=delimiter, lineterminator="\n")
    return writer, f

def read_header(reader: csv.reader) -> Optional[List[str]]:
    try:
        return next(reader)
    except StopIteration:
        return None

def main() -> int:
    args = parse_args()

    in_paths = [Path(p) for p in args.input]
    out_path = Path(args.output)

    if len(in_paths) == 0:
        print("[ERROR] 入力ファイルが指定されていません。-i を1つ以上指定してください。", file=sys.stderr)
        return 2

    # 入出力同一パスの簡易チェック
    for p in in_paths:
        try:
            if p.resolve() == out_path.resolve():
                print(f"[ERROR] 入力と出力が同一パスです: {p}", file=sys.stderr)
                return 2
        except Exception:
            pass

    # 1つ目のファイルを開いてヘッダ採用
    first_reader, first_fp = open_reader(in_paths[0], args.encoding, args.delimiter)
    try:
        first_header = read_header(first_reader)
        if first_header is None:
            print(f"[WARN] 最初の入力が空ファイルです: {in_paths[0]}", file=sys.stderr)
            first_header = []
        writer, out_fp = open_writer(out_path, args.encoding, args.delimiter, args.newline, args.bom == "on")
        try:
            # 1行目: ヘッダを書き出し
            writer.writerow(first_header)

            # 1つ目のファイルの2行目以降を書き出し
            for row in first_reader:
                writer.writerow(row)

            # 2つ目以降のファイル
            for path in in_paths[1:]:
                reader, fp = open_reader(path, args.encoding, args.delimiter)
                try:
                    hdr = read_header(reader)  # ヘッダ読み飛ばし
                    if args.warn_header_mismatch and hdr is not None and len(first_header) != len(hdr):
                        print(f"[WARN] ヘッダ列数不一致: {path} (expected {len(first_header)}, got {len(hdr)})", file=sys.stderr)

                    for row in reader:
                        writer.writerow(row)
                finally:
                    fp.close()
        finally:
            out_fp.close()
    finally:
        first_fp.close()

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
