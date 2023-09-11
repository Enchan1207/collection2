#!/usr/bin/env python3
#
# 出力キャプチャ
#
import argparse
import select
import socket
import subprocess
import sys


def main() -> int:
    # QEMUに渡すオプションとその他オプションをまとめて受け取る
    parser = argparse.ArgumentParser(prog="capture")
    parser.add_argument("--machine", "-M", required=True)
    parser.add_argument("-bios", required=True)
    args = parser.parse_args()

    # ソケットサーバをたてる
    server_address = ('localhost', 61092)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind(server_address)
        server_socket.listen()

        # 引数とオプションを整理してQEMU起動
        qemu_avr = "qemu-system-avr"
        machine = args.machine
        elf_path = args.bios
        subprocess_args = [qemu_avr, "-M", machine, "-bios", elf_path, "-nographic", "-serial",
                           f"tcp:{server_address[0]}:{server_address[1]},server=off"]
        process = subprocess.Popen(args=subprocess_args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)

        # サーバはQEMUクライアントからの接続を受け付ける
        client_socket, _ = server_socket.accept()

        # タイムアウトするまで拾い続ける
        response: bytes = b''
        timed_out = False
        while not timed_out:
            rl, _, _ = select.select([client_socket], [], [], 2)
            if len(rl) == 0:
                timed_out = True
                break
            response += client_socket.recv(64)

        # レスポンスが途切れたらクライアントソケットを閉じる
        client_socket.close()

    # "q\n" を送信してQEMUを終了
    try:
        process.communicate(b'q\n')
    except:
        process.kill()
        return 1

    # 得られたレスポンスをデコードし、最終行を取得
    lastresponseline = response.decode().split("\r\n")[-2]
    if lastresponseline != "All testcases passed.":
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
