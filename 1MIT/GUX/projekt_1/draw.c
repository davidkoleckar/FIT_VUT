/*
 * draw.c - 1. project
 * David Koleckar - xkolec07
 */

/*
 * Standard XToolkit and OSF/Motif include files.
 */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h> 

/*
 * Public include files for widgets used in this file.
 */
#include <Xm/MainW.h> 
#include <Xm/Form.h> 
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Protocols.h>
#include <X11/Xmu/Editres.h>
#include <Xm/MessageB.h>
#include <Xm/ToggleB.h>


/*
 * Common C library include files
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RECTANGLE 23
#define ELLIPSE 24
#define POINT 25
#define LINE 26

/*
 * Shared variables
 */
Widget question, drawArea;
GC drawGC = 0;			/* GC used for final drawing */
GC inputGC = 0;			/* GC used for drawing current position */
int button_pressed = 0;   /* input state */
int x1, y1, x2, y2;		/* input points */ 
XColor linefg, fillfg;
XColor linebg, fillbg;

bool fill = false;
unsigned int lineWid = 3;
unsigned int lineStyle = LineSolid;
unsigned int objectType = LINE;

struct saveDraw{
  int objectType;
  GC drawGC;
  int x1, y1, x2, y2;
  bool fill;
  XColor fillbg;
  XColor fillfg;
};

struct saveDraw *saveDraws;
int size = 0;

/*
 * Input event handler
 */
void inputEH(Widget w, XtPointer client_data, XEvent *event, Boolean *cont)
{
  Pixel	fg, bg;
  int start_x,old_x,start_y,old_y;
  if (button_pressed) {
	  if (!inputGC) {
	    inputGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
	  }
      
    XSetPlaneMask(XtDisplay(w), inputGC, ~0);
    XtVaGetValues(w, XmNforeground, &fg, XmNbackground, &bg, NULL);
    XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
    XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
    XSetLineAttributes(XtDisplay(w), inputGC, lineWid, lineStyle, CapButt, JoinMiter);

  	if (button_pressed > 1) {
      XSetFunction(XtDisplay(w), inputGC, GXxor);
  	    /* erase previous position */
        switch (objectType) {
          case LINE:
            XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
            break;
          case RECTANGLE:
            if (x1 < x2) {
                start_x = x1;
                old_x = x2;
            } else {
                start_x = x2;
                old_x = x1;
            }
            if (y1 < y2) {
                start_y = y1;
                old_y = y2;
            } else {
                start_y = y2;
                old_y = y1;
            }
            if(fill){
              XSetForeground(XtDisplay(w), inputGC, bg ^ fillfg.pixel);
              XSetBackground(XtDisplay(w), inputGC, bg ^ fillbg.pixel);
              XFillRectangle(XtDisplay(w), XtWindow(w), inputGC, start_x,start_y,old_x-start_x,old_y-start_y);
              XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
              XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
            }
            XDrawRectangle(XtDisplay(w), XtWindow(w), inputGC, start_x,start_y,old_x-start_x,old_y-start_y);
            break;
          case ELLIPSE:
            if(fill){
              XSetForeground(XtDisplay(w), inputGC, bg ^ fillfg.pixel);
              XSetBackground(XtDisplay(w), inputGC, bg ^ fillbg.pixel);
              XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
              XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
              XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
            }
            XDrawArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
            break;
          default:
            break;
        }
  	} else {
  	    /* remember first MotionNotify */
  	  button_pressed = 2;
  	}

  	x2 = event->xmotion.x;
  	y2 = event->xmotion.y;
  
  	switch (objectType) {
      case LINE:
        XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
        break;
      case RECTANGLE:
        if (x1 < x2) {
            start_x = x1;
            old_x = x2;
        } else {
            start_x = x2;
            old_x = x1;
        }
        if (y1 < y2) {
            start_y = y1;
            old_y = y2;
        } else {
            start_y = y2;
            old_y = y1;
        }
        if(fill){
          XSetForeground(XtDisplay(w), inputGC, bg ^ fillfg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ fillbg.pixel);
          XFillRectangle(XtDisplay(w), XtWindow(w), inputGC, start_x,start_y,old_x-start_x,old_y-start_y);
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
        }
        XDrawRectangle(XtDisplay(w), XtWindow(w), inputGC, start_x,start_y,old_x-start_x,old_y-start_y);
        break;
      case ELLIPSE:
        if(fill){
          XSetForeground(XtDisplay(w), inputGC, bg ^ fillfg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ fillbg.pixel);
          XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
        }
        XDrawArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
        break;
      case POINT:
        if(lineWid == 0){
          XDrawPoint(XtDisplay(w), XtWindow(w), inputGC, x1, y1);
        }
        else if(lineWid == 3){
          x2 = x1 + 3; y2 = y1 + 3;
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
          XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
        }
        else if(lineWid == 8){
          x2 = x1 + 8; y2 = y1 + 8;
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
          XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), inputGC, bg ^ linefg.pixel);
          XSetBackground(XtDisplay(w), inputGC, bg ^ linebg.pixel);
        }
      default:
        break;
        }
      }
}

/*
 * "Draw" callback function
 */
void DrawCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  XmDrawingAreaCallbackStruct *d = (XmDrawingAreaCallbackStruct*) call_data;

  switch (d->event->type) {
    case ButtonPress:
      if (d->event->xbutton.button == Button1) {
	      button_pressed = 1;
	      x1 = d->event->xbutton.x;
	      y1 = d->event->xbutton.y;
      }
      break;

   case ButtonRelease:
    if (d->event->xbutton.button == Button1) {
    size++;
    saveDraws = (struct saveDraw*) realloc(saveDraws, size * sizeof(struct saveDraw));
    if (saveDraws == NULL) {
      fprintf(stderr, "Error allocate memory!\n");
      exit(1);
    }

    saveDraws[size - 1].drawGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
    XSetForeground(XtDisplay(w), saveDraws[size - 1].drawGC, linefg.pixel);
    XSetBackground(XtDisplay(w), saveDraws[size - 1].drawGC, linebg.pixel);
    XCopyGC(XtDisplay(w), inputGC, GCLineWidth | GCLineStyle, saveDraws[size - 1].drawGC);
    saveDraws[size - 1].objectType = objectType;
    saveDraws[size - 1].x1 = x1;
    saveDraws[size - 1].y1 = y1;
    saveDraws[size - 1].x2 = x2;
    saveDraws[size - 1].y2 = y2;
    saveDraws[size - 1].fill = fill;
    saveDraws[size - 1].fillfg = fillfg;
    saveDraws[size - 1].fillbg = fillbg;

		button_pressed = 0;
    XClearArea(XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True);  
    }
    break;
  }//end-switch
}

/*
 * "Expose" callback function
 */
void ExposeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  XGCValues f,b,wid;
  int start_x,old_x,start_y,old_y;
  for(int i = 0; i < size; i++){
    x1 = saveDraws[i].x1;
    y1 = saveDraws[i].y1;
    x2 = saveDraws[i].x2;
    y2 = saveDraws[i].y2;
    XGetGCValues(XtDisplay(w), saveDraws[i].drawGC, GCForeground, &f);
    XGetGCValues(XtDisplay(w), saveDraws[i].drawGC, GCBackground, &b);

    switch (saveDraws[i].objectType) {
      case LINE:
        XDrawLine(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1, y1, x2, y2);
        break;
      case RECTANGLE:
        if (x1 < x2) {
            start_x = x1;
            old_x = x2;
        } else {
            start_x = x2;
            old_x = x1;
        }
        if (y1 < y2) {
            start_y = y1;
            old_y = y2;
        } else {
            start_y = y2;
            old_y = y1;
        }
        if(saveDraws[i].fill){
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, saveDraws[i].fillfg.pixel);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, saveDraws[i].fillbg.pixel);
          XFillRectangle(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, start_x,start_y,old_x-start_x,old_y-start_y);
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
        }
        XDrawRectangle(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, start_x,start_y,old_x-start_x,old_y-start_y);
        break;
      case ELLIPSE:
        if(saveDraws[i].fill){
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, saveDraws[i].fillfg.pixel);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, saveDraws[i].fillbg.pixel);
          XFillArc(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
        }
        XDrawArc(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
        break;
      case POINT:
        XGetGCValues(XtDisplay(w), saveDraws[i].drawGC, GCLineWidth, &wid);
        if(wid.line_width == 0){
          XDrawPoint(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1, y1);
        }
        else if(wid.line_width == 3){
          x2 = x1 + 3; y2 = y1 + 3;
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
          XFillArc(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
        }
        else if(wid.line_width == 8){
          x2 = x1 + 8; y2 = y1 + 8;
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
          XFillArc(XtDisplay(w), XtWindow(w), saveDraws[i].drawGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), 2*abs(x2 - x1), 2*abs(y2 - y1), 0, 360 * 64);
          XSetForeground(XtDisplay(w), saveDraws[i].drawGC, f.foreground);
          XSetBackground(XtDisplay(w), saveDraws[i].drawGC, b.background);
        }
        break;
      default:
        break;
    }
  }
}

void ClearCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
    Widget wcd = (Widget) client_data;
    size = 0;
    XClearWindow(XtDisplay(wcd), XtWindow(wcd));
}

void questionCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  switch ((intptr_t)client_data)
  {
    case 0: /* ok */
      exit(0);
      break;
    case 1: /* cancel */
      break;
  }
}

void objectCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  switch ((intptr_t)client_data)
  {
    case 0: 
      objectType = LINE;
      fill = false;
      break;
    case 1: 
      objectType = ELLIPSE;
      fill = false;
      break;
    case 2: 
      objectType = ELLIPSE;
      fill = true;
      break;
    case 3: 
      objectType = RECTANGLE;
      fill = false;
      break;
    case 4:
      objectType = RECTANGLE;
      fill = true;
      break;
    case 5:
      objectType = POINT;
      fill = false;
      break;
  }
}

void lineWidthCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  switch ((intptr_t)client_data)
  {
    case 0: 
      lineWid = 0;
      break;
    case 1: 
      lineWid = 3;
      break;
    case 2: 
      lineWid = 8;
      break;
  }
}

void lineTypeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  switch ((intptr_t)client_data)
  {
    case 0: 
      lineStyle = LineSolid;
      break;
    case 1: 
      lineStyle = LineDoubleDash;
      break;
  }
}

void QuitCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
  XtManageChild(question);
}

void linecolorCB( Widget w, XtPointer client_data, XtPointer call_data)   
{
  Colormap screen_colormap;
  screen_colormap = DefaultColormap(XtDisplay(drawArea), DefaultScreen(XtDisplay(drawArea)));
  switch ((intptr_t)client_data)
  {
    case 0: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "black", &linefg, &linefg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "black", &linebg, &linebg);
      break;
    case 1: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "red", &linefg, &linefg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "red", &linebg, &linebg);
      break;
    case 2: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "green", &linefg, &linefg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "green", &linebg, &linebg);
      break;
    case 3: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "blue", &linefg, &linefg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "blue", &linebg, &linebg);
      break;
    case 4: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "white", &linefg, &linefg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "white", &linebg, &linebg);
      break;
  }
}

void fillcolorCB( Widget w, XtPointer client_data, XtPointer call_data)   
{
  Colormap screen_colormap;
  screen_colormap = DefaultColormap(XtDisplay(drawArea), DefaultScreen(XtDisplay(drawArea)));
  switch ((intptr_t)client_data)
  {
    case 0: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "black", &fillfg, &fillfg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "black", &fillbg, &fillbg);
      break;
    case 1: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "red", &fillfg, &fillfg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "red", &fillbg, &fillbg);
      break;
    case 2: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "green", &fillfg, &fillfg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "green", &fillbg, &fillbg);
      break;
    case 3: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "blue", &fillfg, &fillfg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "blue", &fillbg, &fillbg);
      break;
    case 4: 
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "white", &fillfg, &fillfg);
      XAllocNamedColor(XtDisplay(drawArea), screen_colormap, "white", &fillbg, &fillbg);
      break;
  }
}

void fileCB(Widget w, XtPointer client_data, XtPointer call_data)   
{
  switch ((intptr_t)client_data)
  {
    case 0: /* clear */
      size = 0;
      XClearWindow(XtDisplay(drawArea), XtWindow(drawArea));
      break;
    case 1: /* quit */
      XtManageChild(question);
      break;
  }

}

int main(int argc, char **argv)
{
    XtAppContext app_context;
    Atom wm_delete;

    Widget topLevel, mainWin, frame, rowColumn;

    char *fall[] = {
      "*question.dialogTitle: Otázka",
      "*question.messageString: Konec aplikace?",
      "*question.okLabelString: Ano",
      "*question.cancelLabelString: Ne",
      "*question.messageAlignment: XmALIGNMENT_CENTER",
      NULL
    };

    XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

    topLevel = XtVaAppInitialize(
      &app_context,		 	/* Application context */
      "Draw",				/* Application class */
      NULL, 0,				/* command line option list */
      &argc, argv,			/* command line args */
      fall,				/* for missing app-defaults file */
      XmNdeleteResponse, XmDO_NOTHING,
      NULL);				/* terminate varargs list */

    mainWin = XtVaCreateManagedWidget(
      "mainWin",			/* widget name */
      xmMainWindowWidgetClass,		/* widget class */
      topLevel,				/* parent widget*/
      XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
      NULL);				/* terminate varargs list */

    frame = XtVaCreateManagedWidget(
      "frame",				/* widget name */
      xmFrameWidgetClass,		/* widget class */
      mainWin,				/* parent widget */
      NULL);				/* terminate varargs list */

    drawArea = XtVaCreateManagedWidget(
      "drawingArea",			/* widget name */
      xmDrawingAreaWidgetClass,		/* widget class */
      frame,				/* parent widget*/
      XmNwidth, 400,			/* set startup width */
      XmNheight, 400,			/* set startup height */
      NULL);				/* terminate varargs list */

        
    rowColumn = XtVaCreateManagedWidget(
      "rowColumn",			/* widget name */
      xmRowColumnWidgetClass,		/* widget class */
      mainWin,				/* parent widget */
      XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
      XmNorientation, XmHORIZONTAL,	/* orientation */
      XmNpacking, XmPACK_COLUMN,	/* packing mode */
      NULL);				/* terminate varargs list */
        

    Widget menubar,menuColor,menuColorF,widget,menuObject,menuLineType,menuLine;
    XmString colorBg, colorf, none, clear, quit, red, green, blue, black,file, object, line, lineType;
    String  colors[] = {"Black", "Red", "Green", "Blue", "White"};

    file = XmStringCreateLocalized ("File");
    colorBg = XmStringCreateLocalized ("Color line");
    object = XmStringCreateLocalized ("Object");
    line = XmStringCreateLocalized ("Line width");
    lineType = XmStringCreateLocalized ("Line type");
    colorf = XmStringCreateLocalized ("Color fill");

    menubar = XmVaCreateSimpleMenuBar (mainWin, "menubar",        
                                    XmVaCASCADEBUTTON, file, 'F',
                                    XmVaCASCADEBUTTON, colorBg, 'C',
                                    XmVaCASCADEBUTTON, object, 'O',
                                    XmVaCASCADEBUTTON, line, 'L',
                                    XmVaCASCADEBUTTON, lineType, 'l',
                                    XmVaCASCADEBUTTON, colorf, 'l',
                                    NULL);
    XmStringFree (file); 
    XmStringFree (colorBg); 
    XmStringFree (object); 
    XmStringFree (line); 
    XmStringFree (lineType); 
    XmStringFree (colorf); 

    /* File menu */
    quit = XmStringCreateLocalized ("Quit");
    clear = XmStringCreateLocalized ("Clear");
    XmVaCreateSimplePulldownMenu (menubar, "file_menu", 0, fileCB,        
                              XmVaPUSHBUTTON, clear, 'c', NULL, NULL,        
                              XmVaSEPARATOR,        
                              XmVaPUSHBUTTON, quit, 'Q', NULL, NULL,        
                              NULL);    
    XmStringFree (clear);    
    XmStringFree (quit); 

    /* Color menu */    
    black = XmStringCreateLocalized (colors[0]);
    red = XmStringCreateLocalized (colors[1]);
    green = XmStringCreateLocalized (colors[2]);
    blue = XmStringCreateLocalized (colors[3]);
    none = XmStringCreateLocalized (colors[4]);
    menuColor = XmVaCreateSimplePulldownMenu (menubar, "color_menu", 1, linecolorCB,        
                                XmVaRADIOBUTTON, black, 'k', NULL, NULL,
                                XmVaRADIOBUTTON, red, 'R', NULL, NULL,
                                XmVaRADIOBUTTON, green, 'G', NULL, NULL,
                                XmVaRADIOBUTTON, blue, 'B', NULL, NULL,
                                XmVaRADIOBUTTON, none, 'n', NULL, NULL,
                                /* RowColumn resources to enforce */
                                XmNradioBehavior, True,
                                /* radio behavior in Menu */ 
                                XmNradioAlwaysOne, True,
                                NULL);

    //Initialize menu so that "black" is selected.
    if ((widget = XtNameToWidget (menuColor, "button_0")) != (Widget) 0)
              XtVaSetValues (widget, XmNset, XmSET, NULL);
    /* Object menu */
    String  objects[] = {"Line", "Ellipse", "Rectangle", "Point", "Ellipse fill", "Rectangle fill"};
    XmString sLine,sEllipse,sSquare,sPoint, sRectanglefill, sEllipsefill;
    sLine = XmStringCreateLocalized (objects[0]);
    sEllipse = XmStringCreateLocalized (objects[1]);
    sSquare = XmStringCreateLocalized (objects[2]);
    sPoint = XmStringCreateLocalized (objects[3]);
    sEllipsefill = XmStringCreateLocalized (objects[4]);
    sRectanglefill = XmStringCreateLocalized (objects[5]);
    menuObject = XmVaCreateSimplePulldownMenu (menubar, "object_menu", 2, objectCB,        
                                XmVaRADIOBUTTON, sLine, 'l', NULL, NULL,
                                XmVaRADIOBUTTON, sEllipse, 'e', NULL, NULL,
                                XmVaRADIOBUTTON, sEllipsefill, 'u', NULL, NULL,
                                XmVaRADIOBUTTON, sSquare, 's', NULL, NULL,
                                XmVaRADIOBUTTON, sRectanglefill, 'p', NULL, NULL,
                                XmVaRADIOBUTTON, sPoint, 'p', NULL, NULL,
                                XmNradioBehavior, True,
                                XmNradioAlwaysOne, True,
                                NULL);
    XmStringFree (sLine);
    XmStringFree (sEllipse);
    XmStringFree (sSquare);
    XmStringFree (sPoint);
    XmStringFree (sEllipsefill);
    XmStringFree (sRectanglefill);
    if ((widget = XtNameToWidget (menuObject, "button_0")) != (Widget) 0)
              XtVaSetValues (widget, XmNset, XmSET, NULL);
    /* Line width menu */
    String  lineWidth[] = {"0", "3", "8"};
    XmString zero,three,eight;
    zero = XmStringCreateLocalized (lineWidth[0]);
    three = XmStringCreateLocalized (lineWidth[1]);
    eight = XmStringCreateLocalized (lineWidth[2]);

    menuLine = XmVaCreateSimplePulldownMenu (menubar, "line_width_menu", 3, lineWidthCB,        
                                XmVaRADIOBUTTON, zero, 'z', NULL, NULL,
                                XmVaRADIOBUTTON, three, 't', NULL, NULL,
                                XmVaRADIOBUTTON, eight, 'e', NULL, NULL,
                                XmNradioBehavior, True,
                                XmNradioAlwaysOne, True,
                                NULL);
    XmStringFree (zero);
    XmStringFree (three);
    XmStringFree (eight);
    if ((widget = XtNameToWidget (menuLine, "button_1")) != (Widget) 0)
              XtVaSetValues (widget, XmNset, XmSET, NULL);

    /* Linetype menu */

    String  lineTypes[] = {"Solid", "Dashed"};
    XmString solid,dashed;
    solid = XmStringCreateLocalized (lineTypes[0]);
    dashed = XmStringCreateLocalized (lineTypes[1]);

    menuLineType = XmVaCreateSimplePulldownMenu (menubar, "line_type_menu", 4, lineTypeCB,        
                                XmVaRADIOBUTTON, solid, 's', NULL, NULL,
                                XmVaRADIOBUTTON, dashed, 'd', NULL, NULL,
                                XmNradioBehavior, True,
                                XmNradioAlwaysOne, True,
                                NULL);
    XmStringFree (solid);
    XmStringFree (dashed);
    if ((widget = XtNameToWidget (menuLineType, "button_0")) != (Widget) 0)
              XtVaSetValues (widget, XmNset, XmSET, NULL);

    /* color fill */
    menuColorF = XmVaCreateSimplePulldownMenu (menubar, "colorf_menu", 5, fillcolorCB,        
                                XmVaRADIOBUTTON, black, 'k', NULL, NULL,
                                XmVaRADIOBUTTON, red, 'R', NULL, NULL,
                                XmVaRADIOBUTTON, green, 'G', NULL, NULL,
                                XmVaRADIOBUTTON, blue, 'B', NULL, NULL,
                                XmVaRADIOBUTTON, none, 'n', NULL, NULL,
                                /* RowColumn resources to enforce */
                                XmNradioBehavior, True,
                                /* radio behavior in Menu */ 
                                XmNradioAlwaysOne, True,
                                NULL);

    //Initialize menu so that "black" is selected.
    if ((widget = XtNameToWidget (menuColorF, "button_0")) != (Widget) 0)
              XtVaSetValues (widget, XmNset, XmSET, NULL);
    XmStringFree (black);
    XmStringFree (red);
    XmStringFree (green);
    XmStringFree (blue);
    XmStringFree (none);

    XtManageChild (menubar); 
    XmMainWindowSetAreas(mainWin, NULL, rowColumn, NULL, NULL, frame);

    XtAddCallback(drawArea, XmNinputCallback, DrawCB, drawArea);
    XtAddEventHandler(drawArea, ButtonMotionMask, False, inputEH, NULL);
    XtAddCallback(drawArea, XmNexposeCallback, ExposeCB, drawArea);

    /* QUIT question */
    question = XmCreateQuestionDialog(topLevel, "question", NULL, 0);
    XtVaSetValues(question,XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,NULL);
    XtUnmanageChild(XmMessageBoxGetChild(question, XmDIALOG_HELP_BUTTON));
    XtAddCallback(question, XmNokCallback, questionCB, (XtPointer)0);
    XtAddCallback(question, XmNcancelCallback, questionCB, (XtPointer)1);

    wm_delete = XInternAtom(XtDisplay(topLevel), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(topLevel, wm_delete, QuitCB, NULL);
    XmActivateWMProtocol(topLevel, wm_delete);

    XtRealizeWidget(topLevel);
    XtAppMainLoop(app_context);
    return 0;
}

