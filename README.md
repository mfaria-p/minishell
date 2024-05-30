# Minishell
[GNU Bash manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/) <br/>
readline: libreadline-dev 

### Minishell Checklist

- [ ] Display a **prompt** when waiting for a new command :eggplant:
- [ ] Have a working history
- [ ] Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)
- [ ] Avoid using more than one global variable to indicate a received signal
- [ ] Not interpret unclosed quotes or special characters which are not required by the subject
- [ ] Handle `'` which should prevent the shell from interpreting the meta-characters in the quoted sequence
- [ ] Hande `"` which should prevent the shell from interpreting the meta-characters in the quoted sequence except for $
- [ ] Implement redirections:
    - `<` should redirect input
    - `>` should redirect output
    - `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, doesn't have to update the history!
    - `>>` should redirect output in append mode
- [ ] Implement pipes `|`
- [ ] Handle environment variables (`$` followed by a sequence of characters)
- [ ] Handle `$?`
- [ ] Handle `C-c`, `C-d`, `C-\`
    - `C-c` displays a new prompt on a new line
    - `C-d` exits the shell
    - `C-\` does nothing
- [ ] Implement `echo` with option -n
- [ ] Implement `cd` with only a relative or absolute path
- [ ] Implement `pwd` with no options
- [ ] Implement `export` with no options
- [ ] Implement `unset` with no options
- [ ] Implement `env` with no options or arguments
- [ ] Implement `exit` with no options
- [ ] Memory leak free
