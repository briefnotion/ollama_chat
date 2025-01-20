import time

word = "Hello "
count = 0
max_count = 100

while count < max_count:
  print(word, end='', flush=True)
  count += 1
  time.sleep(0.05)  # Sleep for 10 milliseconds

print("Done")
