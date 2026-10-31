import argparse
import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt


def read_journal(path):
    spins, player, cashbox = [], [], []
    staked = paid = 0
    with open(path, newline="", encoding="utf-8") as file:
        for row in csv.DictReader(file):
            spin = int(row["spin"])
            if spin == 1:
                staked += int(row["amount"])
                paid += int(row["payout"])
            if spins and spins[-1] == spin:
                player[-1] = int(row["player_balance"])
                cashbox[-1] = int(row["cashbox_balance"])
            else:
                spins.append(spin)
                player.append(int(row["player_balance"]))
                cashbox.append(int(row["cashbox_balance"]))
    if not spins:
        raise SystemExit(f"Журнал пуст: {path}")
    spins.insert(0, 0)
    player.insert(0, player[0] + staked - paid)
    cashbox.insert(0, cashbox[0] - staked + paid)
    return spins, player, cashbox


def plot_balances(path, out_dir):
    spins, player, cashbox = read_journal(path)
    fig, (top, bottom) = plt.subplots(2, 1, sharex=True, figsize=(9, 6))
    top.plot(spins, player, color="tab:blue")
    top.set_ylabel("Счёт игрока")
    top.set_title(f"Балансы по вращениям: {Path(path).name}")
    bottom.plot(spins, cashbox, color="tab:red")
    bottom.set_ylabel("Касса казино")
    bottom.set_xlabel("Вращение")
    for axis in (top, bottom):
        axis.grid(alpha=0.3)
    fig.tight_layout()
    fig.savefig(out_dir / "balances.png", dpi=120)
    plt.close(fig)


def plot_cumulative(paths, out_dir):
    fig, axis = plt.subplots(figsize=(9, 5))
    for path in paths:
        spins, player, _ = read_journal(path)
        result = [balance - player[0] for balance in player]
        axis.plot(spins, result, label=Path(path).stem)
    axis.axhline(0, color="gray", linewidth=0.8)
    axis.set_title("Накопленный результат игрока")
    axis.set_xlabel("Вращение")
    axis.set_ylabel("Выигрыш (+) / проигрыш (-)")
    axis.grid(alpha=0.3)
    axis.legend()
    fig.tight_layout()
    fig.savefig(out_dir / "cumulative.png", dpi=120)
    plt.close(fig)


def main():
    parser = argparse.ArgumentParser(description="Графики по журналам рулетки")
    parser.add_argument("journals", nargs="+", help="CSV-файлы журналов")
    parser.add_argument("--out", default=".", help="папка для картинок")
    args = parser.parse_args()

    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)
    plot_balances(args.journals[0], out_dir)
    plot_cumulative(args.journals, out_dir)
    print(f"Графики сохранены в {out_dir}/balances.png и {out_dir}/cumulative.png")


if __name__ == "__main__":
    main()
