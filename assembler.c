#include <stdio.h>
#include <stdlib.h>
#include "macr_struct.h"
#include "preprocess.h"
#include "errors.h"
#include "file_handle.h"
#include "first_pass.h"

int main(int argc, char *argv[]) {
    while (--argc > 0) {

        char *as_file, *firstp_file, *prep_file, *am_file;
        FILE *filepreproc;
        FILE *firstpass;
        macro* head;
        as_file = new_file(argv[argc], ".as");
        copy_file(as_file,argv[argc]);

        /* step 1: removing whitespaces from file lines
         ***note*** the final .am file is without whitespaces */
        firstp_file = remove_whitespaces(as_file);
        filepreproc = fopen(firstp_file, "r");
        /* if all declerations are valid -> proceed to preprocess
         else all errors will be printed -> continue to next file */
        if (!exe_validate_declarations(filepreproc)) {
            fclose(filepreproc);
            remove(firstp_file);
            continue;
        }
        /* step 2: adding all macros to the macro linked list */
        rewind(filepreproc);
         head = add_macro_to_list(filepreproc);
        /* step 3: remove all macro declaration and comment lines from the file*/
        prep_file = declaration_removal(firstp_file);
        if (prep_file == NULL) continue;
        /* step 4: replace mentions of macro names in file with contents */
        am_file =replace_names_with_body(prep_file,head);
        if (am_file == NULL) continue;
        /* releasing allocated memory and removing extra created files after preproc */
        fclose(filepreproc);
        remove(firstp_file); free(firstp_file);
        remove(prep_file); free(prep_file);

        firstpass = fopen(am_file, "r");
        if (firstpass == NULL) {
            print_error_msg(3);
            continue;
        }
        /* ---first pass and second pass --- */
        /* if there any errors in first pass -> print errors and continue to next file */
        /* if there are no errors -> first pass will proceed to second pass */
        if (exe_first_pass(firstpass,am_file,head) !=0)
            continue;
        fclose(firstpass);

    }
    return 0;
}
