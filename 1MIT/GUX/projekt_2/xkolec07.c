/*
 * xkolec07.c - 2. project - Text editor
 * David Koleckar - xkolec07
 */

/*
 * Standard XToolkit and OSF/Motif include files.
 */
#include <Xm/Xm.h> 

/*
 * Public include files for widgets used in this file.
 */
#include <Xm/MainW.h> 
#include <Xm/Form.h> 
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Protocols.h>
#include <X11/Xmu/Editres.h>
#include <Xm/MessageB.h>
#include <Xm/ToggleB.h>
#include <Xm/LabelG.h>
#include <Xm/PanedW.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/FileSB.h>

/*
 * Common C library include files
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>


#define MAXBUF 1000000
/*
 * Shared variables
 */
Widget question, text, dialog_win, searchText;

/*
 * "Quit" button callback function
 */
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

/*
 * "Quit" button callback function
 */
void QuitCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
  XtManageChild(question);
}

/*
 * Cancle button in dialog (close)
 */
void closeFileDialog(Widget w, XtPointer client_data, XtPointer call_data)
{
  XtUnmanageChild(w);
}

/*
 * Read file to text field
 */
void openFile(Widget dialog, XtPointer client_data, XtPointer call_data){
  char *filename;
  char source[MAXBUF+1];
  XmFileSelectionBoxCallbackStruct *cbs;

  cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
  filename = XmStringUnparse(cbs->value, XmFONTLIST_DEFAULT_TAG,
                              XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0,
                              XmOUTPUT_ALL);

  FILE *fp = fopen(filename, "r");
  if (fp != NULL) {
    size_t len = fread(source, sizeof(char), MAXBUF, fp);
    if ( ferror( fp ) != 0 ) {
        fputs("Error reading file", stderr);
    } else {
        source[len++] = '\0';
    }
    fclose(fp);
  }

  XmTextSetString(text, source);
  XtFree(filename);
  XtUnmanageChild(dialog_win);
}

/*
 * Save file
 */
void saveFile(Widget dialog, XtPointer client_data, XtPointer call_data){
  char *filename, *output;
  XmFileSelectionBoxCallbackStruct *cbs;

  cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
  filename = XmStringUnparse(cbs->value, XmFONTLIST_DEFAULT_TAG,
                              XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0,
                              XmOUTPUT_ALL);

  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    perror(filename);
    XtFree(filename);
    return;
  }
  else{
    output = XmTextGetString(text);
    fputs(output, fp);
    fclose(fp);
  }

  XtFree(filename);
  XtUnmanageChild(dialog_win);
}

/*
 * Menu file callback
 */
void fileCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  XmString but, tit;
  switch ((intptr_t)client_data)
  {
    case 0: //open
      dialog_win = XmCreateFileSelectionDialog(text, "Files", NULL, 0);
      XtAddCallback(dialog_win, XmNcancelCallback, closeFileDialog, NULL);
      XtAddCallback(dialog_win, XmNokCallback, openFile,NULL);
      but = XmStringCreateLocalized("Open");
      tit = XmStringCreateLocalized("open file");
      XtVaSetValues (dialog_win, XmNokLabelString, but, XmNdialogTitle, tit, NULL);
      XmStringFree(but);
      XmStringFree(tit);
      XtManageChild(dialog_win);
      break;
    case 1: //save
      dialog_win = XmCreateFileSelectionDialog(text, "Files", NULL, 0);
      XtAddCallback(dialog_win, XmNcancelCallback, closeFileDialog, NULL);
      XtAddCallback(dialog_win, XmNokCallback, saveFile,NULL);
      but = XmStringCreateLocalized("Save");
      tit = XmStringCreateLocalized("save file");
      XtVaSetValues (dialog_win, XmNokLabelString, but, XmNdialogTitle, tit, NULL);
      XmStringFree(but);
      XmStringFree(tit);
      XtManageChild(dialog_win);
      break;
    case 2: //quit
      XtManageChild(question);
      break;
  }
}

/*
 * Menu tools callback
 */
void toolsCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  switch ((intptr_t)client_data)
  {
    case 0: //copy
      XmTextCopy(text, CurrentTime);
      break;
    case 1: //past
      XmTextPaste(text);
      break;
    case 2: //remove all
      XmTextSetString(text, "");
      break;
  }
}

/*
 * Menu search callback
 */
void searchCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  bool found = False;
  char *searchString;
  long int len;
  long int pos = 0;
  long int pos2 = 0;

  switch ((intptr_t)client_data)
  {
    case 0: //search all
      pos2 = XmTextGetLastPosition(text);
      XmTextSetHighlight(text, 0, pos2, XmHIGHLIGHT_NORMAL);

      searchString = XmTextGetString(searchText);
      len = strlen(searchString);
      if(searchString == NULL){
        XtFree(searchString);
        return;
      }

      do {
      found = XmTextFindString(text, pos, searchString,XmTEXT_FORWARD, &pos);
      XmTextSetHighlight(text, pos,pos + len,XmHIGHLIGHT_SELECTED);
      pos++;
      } while(found);

      XtFree(searchString);
      break;
  }
}

/*
 * Main
 */
int main(int argc, char **argv)
{
  XtAppContext app_context;
  Atom wm_delete;
  Widget topLevel, mainWin, menubar, pane, rowColumn, label;


  char *fall[] = {
    "*question.dialogTitle: Malá otázka",
    "*question.messageString: Konec aplikace?",
    "*question.okLabelString: Ano",
    "*question.cancelLabelString: Ne",
    "*question.messageAlignment: XmALIGNMENT_CENTER",
    NULL
  };

  /*
   * Register the default language procedure
   */
  XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

  topLevel = XtVaAppInitialize(
    &app_context,     /* Application context */
    "Text editor",       /* Application class */
    NULL, 0,        /* command line option list */
    &argc, argv,      /* command line args */
    fall,       /* for missing app-defaults file */
    XmNdeleteResponse, XmDO_NOTHING,
    NULL);        /* terminate varargs list */

  mainWin = XtVaCreateManagedWidget(
    "mainWin",      /* widget name */
    xmMainWindowWidgetClass,    /* widget class */
    topLevel,       /* parent widget*/
    XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
    NULL);        /* terminate varargs list */

  XmString file, search, tools;

  file = XmStringCreateLocalized("File");
  search = XmStringCreateLocalized("Search");
  tools = XmStringCreateLocalized("Tools");

  menubar = XmVaCreateSimpleMenuBar(mainWin, "menubar",        
                                  XmVaCASCADEBUTTON, file, 'F',
                                  XmVaCASCADEBUTTON, search, 'S',
                                  XmVaCASCADEBUTTON, tools, 'O',
                                  NULL);
  XmStringFree(file); 
  XmStringFree(search); 
  XmStringFree(tools); 

  /* File menu */
  XmString open, save, quit;
  open = XmStringCreateLocalized("Open");
  save = XmStringCreateLocalized("Save");
  quit = XmStringCreateLocalized("Quit");
  XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0, fileCB,        
                            XmVaPUSHBUTTON, open, 'o', NULL, NULL,
                            XmVaPUSHBUTTON, save, 's', NULL, NULL,        
                            XmVaSEPARATOR,        
                            XmVaPUSHBUTTON, quit, 'q', NULL, NULL,        
                            NULL);    
  XmStringFree(open);    
  XmStringFree(save); 
  XmStringFree(quit); 

  /* Search menu */
  XmString searchall;
  searchall = XmStringCreateLocalized("Search all");
  XmVaCreateSimplePulldownMenu(menubar, "search_menu", 1, searchCB,        
                            XmVaPUSHBUTTON, searchall, 'a', NULL, NULL,      
                            NULL);    
  XmStringFree(searchall);  

  /* Tool menu */
  XmString copy, past, remov;
  copy = XmStringCreateLocalized ("Copy");
  past = XmStringCreateLocalized ("Past");
  remov = XmStringCreateLocalized ("Remove all");

  XmVaCreateSimplePulldownMenu (menubar, "tools_menu", 2, toolsCB,        
                            XmVaPUSHBUTTON, copy, 'c', NULL, NULL,
                            XmVaPUSHBUTTON, past, 'p', NULL, NULL,
                            XmVaPUSHBUTTON, remov, 'r', NULL, NULL,
                            NULL);    
  XmStringFree(copy);      
  XmStringFree(past);
  XmStringFree(remov);

  XtManageChild (menubar); 

  pane = XtVaCreateManagedWidget("pane",xmPanedWindowWidgetClass,mainWin,NULL);
  text = XmCreateScrolledText(pane, "scroll", NULL, 0);
  XtVaSetValues(text,
                XmNeditMode, XmMULTI_LINE_EDIT,
                XmNcolumns, 80,
                XmNrows, 10,
                XmNwordWrap, False,
                XmNmaxLength, 100000000,
                XmNheight, 480,
                XmNwidth, 640,
                XmNpaneMinimum,480,
                NULL);
  XtManageChild(text);

  //search panel
  rowColumn = XtVaCreateManagedWidget(
  "rowColumn",      /* widget name */
  xmRowColumnWidgetClass,   /* widget class */
  pane,        /* parent widget */

  NULL);        /* terminate varargs list */

  label = XmCreateLabelGadget(rowColumn, "Search:",NULL, 0);
  XtManageChild(label);
  searchText = XmCreateTextField(rowColumn, "search_text", NULL, 0);
  XtManageChild(searchText);
  XtManageChild(rowColumn);

  /* QUIT question */
  question = XmCreateQuestionDialog(topLevel, "question", NULL, 0);
  XtVaSetValues(question,XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,NULL);
  XtUnmanageChild(XmMessageBoxGetChild(question, XmDIALOG_HELP_BUTTON));
  XtAddCallback(question, XmNokCallback, questionCB, (XtPointer)0);
  XtAddCallback(question, XmNcancelCallback, questionCB, (XtPointer)1);

  wm_delete = XInternAtom(XtDisplay(topLevel), "WM_DELETE_WINDOW", False);
  XmAddWMProtocolCallback(topLevel, wm_delete, QuitCB, NULL);
  XmActivateWMProtocol(topLevel, wm_delete);

  XmMainWindowSetAreas(mainWin, menubar, NULL, NULL, NULL,NULL);

  XtRealizeWidget(topLevel);

  XtAppMainLoop(app_context);

  return 0;
}

