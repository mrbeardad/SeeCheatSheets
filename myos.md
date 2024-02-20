# Utopia OS

## Shell

### Command

```sh
# command
name [args ...] [Redirection]

# pipeline
[!] command [| command]

# list
pipeline [[<newline> or ; or & or && or ||] pipeline]
```

| Redirection     | Description                                                                                              |
| --------------- | -------------------------------------------------------------------------------------------------------- |
| `[fd]<file`     | redirect fd to file, default fd is 0 (stdin)                                                             |
| `[fd]<<EOF`     | redirect fd to the content from next line to the line contaion EOF, default fd is 0 (stdin)              |
| `[fd]<<<string` | redirect fd to string, default fd is 0 (stdin)                                                           |
| `[fd]>file`     | redirect fd to file, truncate the file if it is exist or else create it first, default fd is 1 (stdout)  |
| `[fd]>>file`    | redirect fd to file, append to the file if it is exist or else create it first, default fd is 1 (stdout) |
| `fd1>&fd2`      | redirect fd1 to fd2                                                                                      |

### Expansion

1. Brace Expansion

   | Before      | After      |
   | ----------- | ---------- |
   | `{a,b,c}`   | `a b c`    |
   | `{a..c}`    | `a b c`    |
   | `{1..3}`    | `1 2 3`    |
   | `{1..3..2}` | `1 3`      |
   | `{01..9}`   | `01 02 03` |

2. Tilde Expansion

   | Before  | After                |
   | ------- | -------------------- |
   | `~`     | `/home/current_user` |
   | `~user` | `/home/user`         |

3. Parameter Expansion

   | Before               | After                          |
   | -------------------- | ------------------------------ |
   | `${parameter}`       | parameter value                |
   | `${!parameter}`      | indirect parameter value       |
   | `${#parameter}`      | length of parameter value      |
   | `${parameter:-word}` | use default value              |
   | `${parameter:=word}` | assign default value           |
   | `${parameter:?word}` | display error if null or unset |
   | `${parameter:+word}` | use alternate value            |
   | `${!prefix*}`        | names matching prefix (string) |
   | `${!prefix@}`        | names matching prefix (table)  |
   | `${!prefix[*]}`      | list of table keys (string)    |
   | `${!prefix[@]}`      | list of table keys (table)     |

4. Command Substitution

   | Before       | After                   |
   | ------------ | ----------------------- |
   | `$(command)` | the stdout of `command` |

5. Arithmetic Expansion

   | Before            | After                               |
   | ----------------- | ----------------------------------- |
   | `$((expression))` | the result of arithmetic expression |

6. Process Substitution

   | Before    | After                                                                             |
   | --------- | --------------------------------------------------------------------------------- |
   | `<(list)` | the name of a input FIFO file under /dev/fd where the stdout of list redirects to |
   | `>(list)` | the name of a output FIFO file under /dev/fd where the stdin of list redirects to |

7. Word Splitting

8. Pathname Expansion

   | Before | After                                     |
   | ------ | ----------------------------------------- |
   | `*`    | all files and directories                 |
   | `**`   | all files, directories and subdirectories |
   | `?`    | matches any single character              |
   | `[]`   | same as `[]` in regular expression        |

9. Quote Removal
   - `' '`: all expansion are disabled
   - `" "`: only parameter expansion, command substitution and arithmetic expansion are enabled
   - `\`: escape character

### Parameter

- data types: string and table
- scopes: local, global and environment

```sh
let name=value       # local scope
let -g name=value    # global scope
let -e name=value    # env scope
unlet name
unlet -g name
unlet -e name

# string
string
'string with single quote'
"string with double quote"
# table
(0 1 2)
([0]=0 [1]=1 [2]=2)
([zero]=0 [one]=1 [two]=2)

# expand to the value of variable name in local scope, global scope or env scope
# "${table[@]}" == "0" "1" "2"
# "${table[*]}" == "0 1 2"
$name ${name}
```

| Special Parameters | Description                                                   |
| ------------------ | ------------------------------------------------------------- |
| `$0`               | executable name                                               |
| `$1` `$2` `$3` ... | positional arguments                                          |
| `$@`               | all positional arguments (table)                              |
| `$*`               | all positional arguments (string)                             |
| `$#`               | count of positional arguments                                 |
| `$$`               | pid of current shell                                          |
| `$?`               | exit status of the most recently executed foreground pipeline |

### Control Flow

```sh
if list; then
  list;
elseif list; then
  list;
else
  list;
end

switch word in
  case pattern[|pattern2]
    list;
end

while list; do
  list;
end

for name in word ... do
  list;
end
```

### Function

```sh
function name()
  list;
end
```

## Kernel

### File System

- /
  - boot/
  - usr/
  - bin/
  - lib/
  - etc/
  - var/
  - run/
  - proc/
  - sys/
  - dev/
  - mnt/
  - tmp/
  - home/
    - username/
      - Downloads/
      - Desktop/
      - Douments/
      - Pictures/
      - Videos/
      - Musics/
      - .config/
      - .cache/
      - .local/share/

---

- fs features
  - copy-on-write
  - dedup
  - snapshots
  - compression
  - encryption

---

- file state
  - path
  - size
  - blocks
  - links
  - type
    - regular file
    - directory
    - symbol link
    - fifo
    - socket
    - character
    - block
    - ...
  - uid
  - access
    - regular file
      - read
      - write
      - execute
    - directory
      - read
      - add
      - change
      - delete
