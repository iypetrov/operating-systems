# General

### Write Hello World

- Create `.sh` file

```bash
touch main.sh
```

- Give it executable permissions

```bash
chmod +x main.sh
```

- Print "Hello World", notice that at the beginning you always have to write the `#!/bin/bash` (shebang) to mark the file as a shell script

```bash
#!/bin/bash

echo "Hello World!"
```

### Read docs

When you want to read how some command works there a few ways. Lets say you are interested in `find` command. You can run:

```bash
man find
```

```bash
info find
```

```bash
find --help
```

```bash
# you don't have access to curl command on the exam, but this is useful when you are learning
curl cheat.sh/find
```

### Pipes

Then concept of pipes(`|`) is simple. When you want to pass the result of one command as an input of the other you "pipe" it.

```bash
# here we pipe the numbers from 1 to 5 to the tr command and what will happen is that the tr command will change
# every occurrence of space to new line
echo "1 2 3 4 5" | tr ' ' ' \n'
```

### Basic operations

Use the following command to read a bit more about how to comapare strings, integers, how to validate if something is a file, directory and so on.

```bash
man test
```

### Variables

There are two good practices for writing variables in bash:

- always surround the variables with `{}` (this is good practice because sometimes `_` causes issues)
- always surround the variables with `{}` with `""` (this is because variables in bash are strings and you use `""`)

```bash
foo="foo"
echo "${foo}"
```

Good to mention is also that you surround with `$()` whn you want to wrap the result if command:

```bash
foo="$(echo "1 2 3 4 5" | tr ' ' '\n' | awk '{sum+=$0} END{printf "%d\n", sum}')"
echo "foo -> ${foo}"
```

There are some default variables in bash that you should be aware of:

- `$#` - this shows the number of arguments that you gave to the command
- `$0` - this shows the name of executed script
- `$1` - this shows the first argument you have to the command, if you want to get the second you write `$2` and so on
- `$@` - this is used to get all arguments

### Loops

The most common loop that you are going to use to solve the bash problems is the `while` loop.
In the majority of cases you want to iterate the result of some command, and for this you use the following syntax:

```bash
while read file; do
    echo "${file}"
done < <(find ~ -maxdepth 1 -type f)
```

### Validate Input

- This is how your validations should look like:
  - Always use [[]] for your ifs
  - When you echo the error message write at the end `>&2`
  - Exit with code different from 0 to indicate that error happened (in bash 0 is the code for success)

```bash
if [[ "$#" -ne 1 ]]; then
  echo "provide 1 arg" >&2
  exit 99
fi
```

# Tips & Tricks

To be able to solve your problems fast, here are some tricks that I found:

- Try to learn `vim`. It is kinda important because you are going to use it on exam. Keep in mind that you don't have to be an expert in `vim` to use it.
  Everything beyonde writing & quiting files `:wq` and naviagtion with `hjkl` is optional. These videos might help:
  - [Intro](https://www.youtube.com/watch?v=X6AR2RMB5tE&list=PLm323Lc7iSW_wuxqmKx_xxNtJC_hJbQ7R)
  - [Horizontal movement](https://www.youtube.com/watch?v=5JGVtttuDQA&list=PLm323Lc7iSW_wuxqmKx_xxNtJC_hJbQ7R&index=2)
  - [Vertical movement](https://www.youtube.com/watch?v=KfENDDEpCsI&list=PLm323Lc7iSW_wuxqmKx_xxNtJC_hJbQ7R&index=3)
- While you are solving a problem echo the value of your variables. When you are sure that you solved the problem, delete them.

```bash
foo="$(echo "1 2 3 4 5" | tr ' ' '\n' | awk '{sum+=$0} END{printf "%d\n", sum}')"
echo "foo -> ${foo}"
```

- Don't write too long commands on a single line. It becomes kinda messy and hard to debug when something goes wrong.
  My rule of thumb is that if you have more than 2 pipes use `\` to split the line.

```bash
echo "1 2 3 4 5" \
    | tr ' ' '\n' \
    | awk '{sum+=$0} END{printf "%d\n", sum}'
```

- When you want to trace the output of the program in-depth you can add `-xv` at the end of shebang.

```bash
#!/bin/bash -xv

echo "Hello World!"
```

- When you are on your exam on both parts Bash & C you will receive an archive where are your problems. This is the process that you have to follow:

```bash
tar -xf 66666.tar.xz # on this way you extract the content from archive
mv 66666 62520 # rename the directory to your FN
# here you solve your problems ...
tar -caf 62520 62520.tar.xz # when you are ready you archive your solutions and you are done
```

# Commands
