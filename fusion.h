#ifndef __FUSION_H__
#define __FUSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "utils.h"
#include "es.h"


void fusion(OFile a, OFile b, OFile dest);

void write_header(OFile dest);
void write_section(OFile source, OFile dest, OneHeader *secHeader);
void write_section_header(OFile dest);
#endif
