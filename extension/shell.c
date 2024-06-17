#include "shell.h"
#include "commands/ls.h"

int main(void) {
    LinkedList* l = create_linked_list(NULL);

    Directory d = {
        "dir",
        l,
        l,
        "./"
    };

    Directory* dir = &d;

    // File f =  { "abc.txt", "", "" };
    // dir_add_file(dir, &f);

    // File f2 =  { "defg.txt", "", "" };
    // dir_add_file(dir, &f2);

    Shell s = {
        dir,
        dir,
        "./",
        stdout
    };

    ls(&s);

    return 0;
}