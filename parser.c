#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 circle: add a circle to the edge matrix - 
	    takes 3 arguments (cx, cy, r)
	 hermite: add a hermite curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 bezier: add a bezier curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 xrotate: create an x-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 yrotate: create an y-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 zrotate: create an z-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  color c;
  c.red = 0;
  c.green = 0;
  c.blue = 255;

  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    if (!strcmp(line, "display")) {
      clear_screen(s);
      draw_lines(pm, s, c);
      display(s);
      //clear_screen(s);
    } else if (!strcmp(line, "apply")) {
      matrix_mult(transform, pm);
      //draw_lines(pm, s, c);
    } else if (!strcmp(line, "ident")) {
      ident(transform);
    } else if (!strcmp(line, "quit")) {
      exit(0);
    } else {
      char args[256];
      fgets(args, 255, f);
      args[strlen(args)-1]='\0';
      char * p_args = args;
      char * inputs[100];

      int i;
      for (i = 0; (inputs[i] = strsep(&p_args, " ")); i++) {
	printf("%s ", inputs[i]);
      }
      if (!strcmp(line, "save")) {
	save_extension(s, "pic.png");
      } else {
	inputs[i] = "-1";
	printf("\n");
	double d_inputs[100];
	for (i = 0; strcmp(inputs[i], "-1") != 0; i++) {
	  d_inputs[i] = strtod(inputs[i], NULL);
	  printf("%f ", d_inputs[i]);
	}
	printf("\n");
	if (!strcmp(line, "line")) {
	  add_edge(pm, d_inputs[0], d_inputs[1], d_inputs[2], d_inputs[3], d_inputs[4], d_inputs[5]);
	}
	else if (!strcmp(line, "circle")) {
	  add_circle(pm, d_inputs[0], d_inputs[1], d_inputs[2], 0.1);
	}
	else if (!strcmp(line, "hermite")) {
	  add_curve(pm, d_inputs[0], d_inputs[1], d_inputs[2], d_inputs[3], d_inputs[4], d_inputs[5],d_inputs[6], d_inputs[7], 0.1, 0);  
	}
	else if (!strcmp(line, "bezier")) {
	  add_curve(pm, d_inputs[0], d_inputs[1], d_inputs[2], d_inputs[3], d_inputs[4], d_inputs[5],d_inputs[6], d_inputs[7], 0.1, 1);
	}
	else if (!strcmp(line, "scale")) {
	  struct matrix * m1 = make_scale(d_inputs[0], d_inputs[1], d_inputs[2]); 
	  matrix_mult(m1, transform);
	  free_matrix(m1);
	}
	else if (!strcmp(line, "translate")) {
	  struct matrix * m1 = make_translate(d_inputs[0], d_inputs[1], d_inputs[2]); 
	  matrix_mult(m1, transform);
	  free_matrix(m1);
	}
	else if (!strcmp(line, "xrotate")) {
	  struct matrix * m1 = make_rotX(d_inputs[0]);
	  matrix_mult(m1, pm);
	  free_matrix(m1);
	}
	else if (!strcmp(line, "yrotate")) {
	  struct matrix * m1 = make_rotY(d_inputs[0]);
	  matrix_mult(m1, pm);
	  free_matrix(m1);	  
	}
	else if (!strcmp(line, "zrotate")) {
	  struct matrix * m1 = make_rotZ(d_inputs[0]);
	  matrix_mult(m1, pm);
	  free_matrix(m1);
	}
      }
    }
      /*    if (line == "line") {
      
	  add_edge(pm,*/ 
    //printf("2nd-> :%s:\n",line);
  }
}

