#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CSV整形ツール（Python 3.10.5）
- '>'で始まるメタ行を削除（デフォルトはスマート判定：数値CSV行っぽいものは残す）
- 空行（全角スペースのみも含む）を削除
- 入出力ファイルと保存先を指定可
- 改行コード/エンコーディング/BOMを選択可
- Windows, macOS, Linux対応
"""

from __future__ import annotations
import argparse
import sys
from pathlib import Path
from typing import TextIO

def is_blank_line(line: str) -> bool:
    # Pythonのstrip()はUnicode空白（全角スペースU+3000含む）を削除対象に含む
    return line.strip() == ""

def looks_like_numeric_csv(s: str) -> bool:
    """
    '数値CSV行っぽい' 判定：カンマを含み、最初の非空白文字が [+-.0-9] のいずれか。
    例: "-0.056000,-1.028000,-0.032000" → True
    """
    ss = s.lstrip()
    if not ss:
        return False
    if "," not in ss:
        return False
    return ss[0] in "+-." or ss[0].isdigit()

def should_drop_line(line: str, strict_gt: bool) -> bool:
    """
    削除対象かどうかを判定
    - 空行は削除
    - '>'で始まる行:
        strict_gt=True  → 常に削除
        strict_gt=False → '数値CSV行っぽくない' ものだけ削除（メタ行想定）
    """
    if is_blank_line(line):
        return True

    s = line.lstrip()
    if s.startswith(">"):
        if strict_gt:
            return True
        # スマート判定： '>'の後ろが数値CSVに見えれば残す
        rest = s[1:]
        return not looks_like_numeric_csv(rest)

    return False

def open_in(path: str, encoding: str) -> TextIO:
    if path == "-" or path == "":
        # 標準入力
        return sys.stdin
    # newline=None で universal newline 読み
    return open(path, "r", encoding=encoding, newline=None)

def open_out(path: str, encoding: str, newline: str, bom: bool) -> TextIO:
    if path == "-" or path == "":
        # 標準出力
        # sys.stdout はencodingが固定のことがある。BOM付与はprintで対応不可のため、
        # 標準出力にBOMを出したい場合は最初に直接書く。
        if bom:
            try:
                # バイナリにBOM直書き（可能なら）
                sys.stdout.buffer.write("\ufeff".encode("utf-8"))
            except Exception:
                # だめならテキストで書けるだけ
                print("\ufeff", end="")
        return sys.stdout

    # newline:
    #   native: OSデフォルト（Pythonは textモード + newline="" でOS改行）
    #   unix:   \n
    #   windows:\r\n
    newline_map = {
        "native": "",
        "unix": "\n",
        "windows": "\r\n",
    }
    nl = newline_map[newline]

    f = open(path, "w", encoding=encoding, newline=nl)
    if bom:
        f.write("\ufeff")
    return f

def main() -> int:
    p = argparse.ArgumentParser(
        prog="csv_clean",
        description="CSV整形：'>'で始まるメタ行と空行を削除（Windows/macOS/Linux対応）"
    )
    p.add_argument("-i", "--input", required=True,
                   help="入力ファイルパス（- で標準入力）")
    p.add_argument("-o", "--output", required=True,
                   help="出力ファイルパス（- で標準出力）")
    p.add_argument("--strict-gt", action="store_true",
                   help="Trueにすると '>'で始まる行を無条件に削除（スマート判定を無効化）")
    p.add_argument("--encoding", default="utf-8",
                   help="入出力の文字コード（既定: utf-8）")
    p.add_argument("--newline", choices=["native", "unix", "windows"], default="native",
                   help="出力改行コード（既定: native=OS準拠）")
    p.add_argument("--bom", choices=["on", "off"], default="off",
                   help="出力先頭にBOM(utf-8)を付与（Excel等向け）。既定: off")
    p.add_argument("-v", "--verbose", action="store_true", help="処理統計などを表示")

    args = p.parse_args()

    in_path = args.input
    out_path = args.output
    strict_gt = args.strict_gt
    encoding = args.encoding
    newline_mode = args.newline
    bom = (args.bom == "on")

    # 入出力が同じパスの場合はエラー（明示的な上書き機能は付けない）
    if in_path != "-" and out_path != "-" and Path(in_path).resolve() == Path(out_path).resolve():
        print("[ERROR] 入力と出力に同じパスを指定しています。別の出力先を指定してください。", file=sys.stderr)
        return 2

    total = kept = dropped = 0
    with open_in(in_path, encoding=encoding) as fin, open_out(out_path, encoding=encoding, newline=newline_mode, bom=bom) as fout:
        for line in fin:
            total += 1
            if should_drop_line(line, strict_gt=strict_gt):
                dropped += 1
                continue
            kept += 1
            # 行末は読み込み時点で \n に正規化されている想定
            fout.write(line)

    if args.verbose:
        print(f"[INFO] total={total} kept={kept} dropped={dropped}", file=sys.stderr)

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
