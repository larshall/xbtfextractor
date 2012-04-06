/*
 * Copyright (C) 2012 Lars Hall
 *
 * This file is part of xbtfextractor.
 *
 * xbtfextractor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xbtfextractor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xbtfextractor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <getopt.h>
#include "xbtf.h"

void printHelp()
{
    fprintf(stdout, "Usage: xbtfextractor [OPTION...] [FILE]             \n");
    fprintf(stdout, "Examples:                                           \n");
    fprintf(stdout, "  xbtfextractor -o ./dir/ -c textures.xbt    "         );
    fprintf(stdout, "# Extracts textures.xbt to\n"                          );
    fprintf(stdout, "                                             "         );
    fprintf(stdout, "# the specified dir\n"                                 );
    fprintf(stdout, "                                             "         );
    fprintf(stdout, "# and creates the directory tree\n"                    );
    fprintf(stdout, "  xbtfextractor -p xbtfile.xbt               "         );
    fprintf(stdout, "# Prints all files/path\n"                             );
    fprintf(stdout, "                                             "         );
    fprintf(stdout, "# in the xbtfile.xbt\n"                                );
    fprintf(stdout, "  xbtfextractor -c -f /path/file xbtfile.xbt "         );
    fprintf(stdout, "# Extracts /path/file from\n"                          );
    fprintf(stdout, "                                             "         );
    fprintf(stdout, "# the xbtfile.xbt\n\n");
    fprintf(stdout, "Options:                                            \n");
    fprintf(stdout, "  -o, --outdir=OUTDIR The directory "                  );
    fprintf(stdout, "where to store the files                            \n");
    fprintf(stdout, "  -c, --create Create directories to store the files\n");
    fprintf(stdout, "  -c, --create Creates the directory tree           \n");
    fprintf(stdout, "  -f, --file=FILENAME  Extracts only the FILENAME   \n");
    fprintf(stdout, "  -p, --print Print all the files in the xbtfile    \n");
    fprintf(stdout, "  -h, --help Prints this                          \n\n");
}

int main(int argc, char **argv)
{
    bool createDirs = false;
    int print = 0;

    char *outDir = NULL;
    char *xbtfFile = NULL;
    char *filename = NULL;

    while(true)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"help",  no_argument,       0, 'h'},
            {"print",  no_argument,       0, 'p'},
            {"create-dirs",  no_argument, 0, 'c'},
            {"outdir",  required_argument, 0, 'o'},
            {"file",  required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long (argc, argv, "hpcf:o:",
            long_options, &option_index);

        // end of input
        if (c == -1)
             break;

        switch (c)
        {
            case 0:
                break;
            case 'h':
                printHelp();
                exit(0);
                break;
            case 'o':
                outDir = optarg;
                break;
            case 'c':
                createDirs = true;
                break;
            case 'f':
                filename = optarg;
                break;
            case 'p':
                print = true;
                break;

            default:
                break;
        }
        // TODO
    }
    if (optind < argc)
    {
        while (optind < argc)
            xbtfFile = argv[optind++];
    }


    if (xbtfFile != NULL)
    {
        Xbtf xbtf;

        if ((createDirs) && (outDir == NULL))
        {
            // Just to be sure the user knows what implications this can have
            fprintf(stderr, "Warning: OUTDIR isn't specified so directories");
            fprintf(stderr, "will be created in current dir. If this is");
            fprintf(stderr, "OK, set \"-o ./\"\n");
            exit(1);
        }
        if (print)
        {
            if (!xbtf.open(xbtfFile))
                return 1;

            xbtf.parse();      
            xbtf.printFiles();
        }
        else
        {
            if (!xbtf.open(xbtfFile))
                return 1;

            xbtf.parse();

            if (filename != NULL)
                xbtf.extractFile(filename, outDir, createDirs);
            else
                xbtf.extractAllFiles(outDir, createDirs);
        }
    }
    else
    {
        fprintf(stderr, "Required option (xbtfile) is missing\n");
        printHelp();
        exit(1);
    }

    return 0;
}
