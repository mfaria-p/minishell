# Minishell
Our own little bash

### Resources

[GNU Bash manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/) <br/>
readline: libreadline-dev 

### Minishell Checklist

- [x] Display a **prompt** when waiting for a new command :eggplant:
- [x] Have a working history
- [x] Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)
- [x] Avoid using more than one global variable to indicate a received signal
- [x] Not interpret unclosed quotes or special characters which are not required by the subject
- [x] Handle `'` which should prevent the shell from interpreting the meta-characters in the quoted sequence
- [x] Hande `"` which should prevent the shell from interpreting the meta-characters in the quoted sequence except for $
- [x] Implement redirections:
    - `<` should redirect input
    - `>` should redirect output
    - `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, doesn't have to update the history!
    - `>>` should redirect output in append mode
- [x] Implement pipes `|`
- [x] Handle environment variables (`$` followed by a sequence of characters)
- [x] Handle `$?`
- [x] Handle `C-c`, `C-d`, `C-\`
    - `C-c` displays a new prompt on a new line
    - `C-d` exits the shell
    - `C-\` does nothing
- [x] Implement `echo` with option -n
- [x] Implement `cd` with only a relative or absolute path
- [x] Implement `pwd` with no options
- [x] Implement `export` with no options
- [x] Implement `unset` with no options
- [x] Implement `env` with no options or arguments
- [x] Implement `exit` with no options
- [x] Memory leak free
