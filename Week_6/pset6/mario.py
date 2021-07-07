def input_height():
    """get pyramid height """

    while True:
        h = input("Height: ")
        if h in "123456789":
            return int(h)


def draw(h):
    """draw pyramid """

    for i in range(1, h + 1):
        print(f"{' ' * (h - i)}{('#' * i)}  {('#' * i)}")


def main():
    """print pyramide"""

    height = input_height()
    draw(height)


if __name__ == "__main__":
    main()
