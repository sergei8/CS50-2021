from functools import wraps
    
def decor(f):
    # @wraps(f)
    def wrap():
        a = "@@@" + f()
        return a
    return wrap

@decor
def f():
    return "aaaa"



if __name__ == "__main__":
    # f = decor(f)
    print (f())



