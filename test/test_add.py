import subprocess as sp


def main():
    a = 1
    b = 2
    ans = sp.run(['./bigint', str(a), str(b)],
                 shell=False, stderr=sp.PIPE, stdout=sp.PIPE)
    print(ans)


if __name__ == "__main__":
    main()
