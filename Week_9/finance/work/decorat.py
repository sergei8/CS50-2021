def f():
    return "aaaa"
    
def decor(f):
    def wrap():
        a = "@@@" + f()
        return a
    return wrap

if __name__ == "__main__":
    f = decor(f)
    print (f())



