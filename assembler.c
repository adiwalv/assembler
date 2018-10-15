// Author : Vikas Adiwal
#include "build/generate_lst.h"

int main(int argc, char *argv[]) {
  int p_flag, s_flag, i_flag, t_flag, l_flag, h_flag;
  p_flag = s_flag = i_flag = t_flag = l_flag = h_flag = 0;
    while ((ch = getopt(argc, argv, "splith")) != -1) {
        if (ch == 's')
            s_flag = 1;
        else if (ch == 'p')
          p_flag = 1;
        else if (ch == 'l')
            l_flag = 1;
        else if (ch == 'i')
            i_flag = 1;
        else if (ch == 't')
            t_flag = 1;
        else if (ch == 'h')
            h_flag = 1;
        else
          continue;
    }
    if (optind != argc - 1) {
      printUsage(argv[0]);
       if (h_flag)
         printHelp();
    } else {
      if (h_flag) {
        printUsage(argv[0]);
        printHelp();
      } else {
          generateTables(argv[optind]);
        if (p_flag)
          printSource(argv[optind]);
        if (s_flag)
          printSymTab(sym_table_index);
        if (i_flag)
          printImmediateCode(op, argv[optind]);
        if (t_flag)
          printLiteralTab(lit_table_index);
        if (l_flag) {
          if (error_table_index > 1) {
            generateLST();
          }
        }
      }
    }
}
