unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, StdCtrls, OleCtrls, CodeMax_TLB;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    Open1: TMenuItem;
    N1: TMenuItem;
    Exit1: TMenuItem;
    CodeMax1: TCodeMax;
    ImageList1: TImageList;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    New1: TMenuItem;
    Save1: TMenuItem;
    SaveAs1: TMenuItem;
    procedure CodeMax1CodeList(Sender: TObject; const Control: ICodeMax;
      const ListCtrl: ICodeList; var pbContinue: WordBool);
    procedure CodeMax1CodeListSelMade(Sender: TObject;
      const Control: ICodeMax; const ListCtrl: ICodeList;
      var pbStop: WordBool);
    procedure Open1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure New1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Save1Click(Sender: TObject);
    procedure SaveAs1Click(Sender: TObject);
    procedure CodeMax1CodeTip(Sender: TObject; const Control: ICodeMax;
      var ToolTipType: TOleEnum);
    procedure CodeMax1CodeTipUpdate(Sender: TObject;
      const Control: ICodeMax; const ToolTipCtrl: ICodeTip);
    procedure CodeMax1KeyPress(Sender: TObject; const Control: ICodeMax;
      KeyAscii, Shift: Integer; var pbStop: WordBool);

  private
    { Private declarations }
    selRange : IRange;
  public
    function SaveModified : boolean;
  end;

var
  Form1: TForm1;
  Filename : string;
  
implementation

{$R *.DFM}



procedure TForm1.CodeMax1CodeList(Sender: TObject;
  const Control: ICodeMax; const ListCtrl: ICodeList;
  var pbContinue: WordBool);
begin
     // Attach our icon imagelist to the list view control
     ListCtrl.hImageList := ImageList1.Handle;

     // Add function names to list view control
     ListCtrl.AddItem( 'Function1', 0, 0 );
     ListCtrl.AddItem( 'Function2', 0, 0 );
     ListCtrl.AddItem( 'Function3', 0, 0 );
     ListCtrl.AddItem( 'Function4', 0, 0 );
     ListCtrl.AddItem( 'Function5', 0, 0 );
     ListCtrl.AddItem( 'Function6', 0, 0 );

     // Just for kicks, we'll select the first item by default...
     ListCtrl.SelectedItem := 0;

     // Allow list view control to be displayed
     pbContinue := true;
end;

procedure TForm1.CodeMax1CodeListSelMade(Sender: TObject;
  const Control: ICodeMax; const ListCtrl: ICodeList;
  var pbStop: WordBool);
var
   strItem : string;
   range : IRange;
begin
     // Determine which item was selected in the list
     strItem := ListCtrl.GetItemText( ListCtrl.SelectedItem );

     // Replace current selection
     Control.ReplaceSel( strItem );

    // Get new selection
    range := Control.GetSel( True );

    // Update range to end of newly inserted text
    range.StartColNo := range.StartColNo + Length(strItem);
    range.EndColNo := range.StartColNo;
    range.EndLineNo := range.StartLineNo;

    // Move cursor
    Control.SetSel( range, true );

    // Don't prevent list view control from being hidden
    pbStop := false;
end;

procedure TForm1.New1Click(Sender: TObject);
var
   range : IRange;
begin
     if SaveModified then begin
        // There's probably a better way to do this, but what...?
        CodeMax1.ExecuteCmd( cmCmdSelectAll, 0 );
        CodeMax1.DeleteSel;
        CodeMax1.SetSel( range, true );
        CodeMax1.ClearUndoBuffer;
        CodeMax1.Modified := false;
        Filename := '';
     end;//if
end;

procedure TForm1.Open1Click(Sender: TObject);
begin
     if SaveModified and OpenDialog1.Execute then begin
        Filename := OpenDialog1.Filename;
        CodeMax1.OpenFile( Filename );
        CodeMax1.ClearUndoBuffer;
        CodeMax1.Modified := false;
     end;//if
end;

procedure TForm1.Save1Click(Sender: TObject);
begin
     if Filename = '' then
        SaveAs1Click( Sender )
     else begin
        CodeMax1.SaveFile( Filename, false );
        CodeMax1.Modified := false;
     end;//if
end;

procedure TForm1.SaveAs1Click(Sender: TObject);
begin
     if SaveDialog1.Execute then begin
        Filename := SaveDialog1.FileName;
        CodeMax1.SaveFile( Filename, false );
        CodeMax1.Modified := false;
     end;//if
end;

procedure TForm1.Exit1Click(Sender: TObject);
begin
     Form1.Close
end;

function TForm1.SaveModified() : boolean;
begin
     SaveModified := true;

     if CodeMax1.Modified then begin
        case MessageDlg('Save changes?', mtConfirmation, mbYesNoCancel, 0) of
             mrYes :
                   if Filename <> '' then
                      begin
                           CodeMax1.SaveFile( Filename, false );
                           CodeMax1.Modified := false;
                      end
                   else begin
                      if SaveDialog1.Execute then
                         begin
                              Filename := SaveDialog1.FileName;
                              CodeMax1.SaveFile( FileName, false );
                              CodeMax1.Modified := false;
                         end
                      else
                         SaveModified := false;
                   end;//if
             mrCancel :
                   SaveModified := false;
        end;//case
     end;//if
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
     Filename := '';
end;

procedure TForm1.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
     if SaveModified then
        CanClose := true
     else
        CanClose := false;
end;

procedure TForm1.CodeMax1CodeTip(Sender: TObject; const Control: ICodeMax;
  var ToolTipType: TOleEnum);
  var token : cmTokenType;
begin

     // We don't want to display a tip inside quoted or commented-out lines...
     token := Control.CurrentToken;

     if( (cmTokenTypeText = token) or (cmTokenTypeKeyword = token) ) then
         // We want to use the tooltip control that automatically
         // highlights the arguments in the function prototypes for us.
         // This type also provides support for overloaded function
         // prototypes.
         ToolTipType := cmToolTipTypeMultiFunc
     else
         ToolTipType := cmToolTipTypeNone;// Don't display a tooltip
end;

procedure TForm1.CodeMax1CodeTipUpdate(Sender: TObject;
  const Control: ICodeMax; const ToolTipCtrl: ICodeTip);

var
   range : IRange;
   i, j, iArg : integer;
   strLine : string;
   iTrim : integer;
   tip : ICodeTipMultiFunc;

begin
     tip := ICodeTipMultiFunc( ToolTipCtrl );

     // See if the tooltip control has been initialized yet
     if( tip.TipTextLength = 0 )then begin

         // Default to first argument
         tip.Argument := 0;

         // Save the starting position for use with the CodeTip.  This is so we
         // can destroy the tip window if the user moves the cursor back before
         // or above the starting point.
         //
         selRange := Control.GetSel( true );
         selRange.EndColNo := selRange.EndColNo + 1;

         // Set number of function overloads
         tip.FunctionCount := 1;

         // Default to first function prototype
         tip.CurrentFunction := 0;

         // Set tooltip text to first function prototype
         tip.TipText := 'Function( Blah, blah, blah... )';

         end
     else begin

          // Destroy the tooltip window if the caret is moved above or before
          // the starting point.
          range := Control.GetSel( true );

          if( (range.EndLineNo < selRange.EndLineNo) or
              ((range.EndColNo < selRange.EndColNo) and
              (range.EndLineNo <= selRange.EndLineNo)) ) then

              // Caret moved too far up / back
              tip.Destroy()

          else begin

              // Determine which argument to highlight
              i := selRange.EndLineNo;
              iArg := 0;

              while( (i <= range.EndLineNo) and (iArg <> -1))do begin

                  // Get next line from buffer   
                  strLine := Control.GetLine( i );

                  if( i = range.EndLineNo )then begin
                      // Trim off any excess beyond cursor pos so the argument with the
                      // cursor in it will be highlighted.
                      iTrim := Length(strLine) + 1;
                      if(range.EndColNo < iTrim)then
                         iTrim := range.EndColNo;
                      strLine := Copy( strLine, 0, iTrim );
                  end;

                  // Parse arguments from current line
                  j:= 0;
                  while( (Length(strLine) <> 0) and (j <= Length(strLine)) and (iArg <> -1) )do begin
                      if( strLine[j] = ',')then
                          iArg := iArg + 1
                      else if( strLine[j] = ')')then
                          iArg := -1;
                      j := j + 1;
                  end;
                  i := i + 1;
              end;

              if( -1 = iArg )then
                  tip.Destroy()// Right-paren found
              else begin
                  tip.Argument := iArg;

                  // Set tooltip text to current function prototype
                  If (tip.CurrentFunction = 0) Then
                     tip.TipText := 'Function( Blah, blah, blah... )'
                  Else
                      tip.TipText := 'Function( Foobar )';
              end;
          end;
     end;
end;

procedure TForm1.CodeMax1KeyPress(Sender: TObject; const Control: ICodeMax;
  KeyAscii, Shift: Integer; var pbStop: WordBool);
begin
     // Invoke the CodeTip feature when the user presses the '(' key. We'll
     // determine whether or not we are actually ready to handle the message
     // in the OnCodeTip handler.
     //
     if( '(' = char(KeyAscii) )then
         Control.ExecuteCmd( cmCmdCodeTip, 0 );
end;

end.

