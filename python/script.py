# script.py
import sys

def main():
    result = "Hello from Python"
    with open("output.txt", "w") as f:
        f.write(result)    
        print("Hello from Python 2")

if __name__ == "__main__":
    main()