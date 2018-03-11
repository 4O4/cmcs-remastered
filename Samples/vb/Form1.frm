VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{ECEDB943-AC41-11D2-AB20-000000000000}#2.0#0"; "cmax20.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3555
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5760
   LinkTopic       =   "Form1"
   ScaleHeight     =   3555
   ScaleWidth      =   5760
   StartUpPosition =   3  'Windows Default
   Begin MSComctlLib.ImageList ImageList2 
      Left            =   3000
      Top             =   2640
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      UseMaskColor    =   0   'False
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   1
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0000
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   4680
      Top             =   2520
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   14
      ImageHeight     =   13
      MaskColor       =   65280
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   3
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":015A
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0254
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":034E
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin CodeMaxCtl.CodeMax CodeMax1 
      Height          =   3255
      Left            =   120
      OleObjectBlob   =   "Form1.frx":0448
      TabIndex        =   0
      Top             =   120
      Width           =   5415
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim selRange As CodeMaxCtl.IRange

Private Function CodeMax1_CodeList(ByVal Control As CodeMaxCtl.ICodeMax, ByVal ListCtrl As CodeMaxCtl.ICodeList) As Boolean

    ' Attach our icon imagelist to the list view control
    ListCtrl.hImageList = ImageList2.hImageList

    ' Add function names to list view control
    ListCtrl.AddItem "Function1"
    ListCtrl.AddItem "Function2"
    ListCtrl.AddItem "Function3"
    ListCtrl.AddItem "Function4"
    ListCtrl.AddItem "Function5"

    ' Just for kicks, we'll select the first item by default...
    ListCtrl.SelectedItem = 0

    ' Allow list view control to be displayed
    CodeMax1_CodeList = True

End Function

Private Function CodeMax1_CodeListSelMade(ByVal Control As CodeMaxCtl.ICodeMax, ByVal ListCtrl As CodeMaxCtl.ICodeList) As Boolean

    Dim strItem As String
    Dim range As New CodeMaxCtl.range

    ' Determine which item was selected in the list
    strItem = ListCtrl.GetItemText(ListCtrl.SelectedItem)

    ' Replace current selection
    CodeMax1.ReplaceSel (strItem)

    ' Get new selection
    Set range = CodeMax1.GetSel(True)

    ' Update range to end of newly inserted text
    range.StartColNo = range.StartColNo + Len(strItem)
    range.EndColNo = range.StartColNo
    range.EndLineNo = range.StartLineNo

    ' Move cursor
    CodeMax1.SetSel range, True

    ' Don't prevent list view control from being hidden
    CodeMax1_CodeListSelMade = False

End Function

Private Function CodeMax1_CodeTip(ByVal Control As CodeMaxCtl.ICodeMax) As CodeMaxCtl.cmToolTipType

    Dim token As CodeMaxCtl.cmTokenType

    ' We don't want to display a tip inside quoted or commented-out lines...
    token = Control.CurrentToken

    If ((cmTokenTypeText = token) Or (cmTokenTypeKeyword = token)) Then
        ' We want to use the tooltip control that automatically
        ' highlights the arguments in the function prototypes for us.
        ' This type also provides support for overloaded function
        ' prototypes.
        CodeMax1_CodeTip = cmToolTipTypeMultiFunc
    Else
        ' Don't display a tooltip
        CodeMax1_CodeTip = cmToolTipTypeNone
    End If

End Function

Private Sub CodeMax1_CodeTipUpdate(ByVal Control As CodeMaxCtl.ICodeMax, ByVal ToolTipCtrl As CodeMaxCtl.ICodeTip)

    Dim tip As CodeMaxCtl.CodeTipMultiFunc
    Set tip = ToolTipCtrl

    ' See if the tooltip control has been initialized yet
    If (tip.TipTextLength = 0) Then

        ' Default to first argument
        tip.Argument = 0

        ' Save the starting position for use with the CodeTip.  This is so we
        ' can destroy the tip window if the user moves the cursor back before
        ' or above the starting point.
        '
        Set selRange = Control.GetSel(True)
        selRange.EndColNo = selRange.EndColNo + 1

        ' Set number of function overloads
        tip.FunctionCount = 1

        ' Default to first function prototype
        tip.CurrentFunction = 0

        ' Set tooltip text to first function prototype
        tip.TipText = "SomeFunc( Blah, blah, blah... )"

    Else

        Dim range As CodeMaxCtl.IRange

        ' Destroy the tooltip window if the caret is moved above or before
        ' the starting point.
        Set range = Control.GetSel(True)

        If ((range.EndLineNo < selRange.EndLineNo) Or _
            ((range.EndColNo < selRange.EndColNo) And _
            (range.EndLineNo <= selRange.EndLineNo))) Then

            ' Caret moved too far up / back
            tip.Destroy

        Else

            ' Determine which argument to highlight
            Dim iArg, i As Integer
            Dim strLine As String

            iArg = 0
            i = selRange.EndLineNo

            While ((i <= range.EndLineNo) And (iArg <> -1))

                'Get next line from buffer
                strLine = Control.GetLine(i)

                If (i = range.EndLineNo) Then
                    ' Trim off any excess beyond cursor pos so the argument with the
                    ' cursor in it will be highlighted.
                    iTrim = Len(strLine) + 1
                    If (range.EndColNo < iTrim) Then
                        iTrim = range.EndColNo
                    End If
                    strLine = Left(strLine, iTrim)
                End If

                ' Parse arguments from current line
                j = 0
                While ((Len(strLine) <> 0) And (j <= Len(strLine)) And (iArg <> -1))
                    If (Mid(strLine, j + 1, 1) = ",") Then
                        iArg = iArg + 1
                    ElseIf (Mid(strLine, j + 1, 1) = ")") Then
                        iArg = -1
                    End If
                    j = j + 1
                Wend

                i = i + 1
            Wend

            If (-1 = iArg) Then
                tip.Destroy 'Right-paren found
            Else
                tip.Argument = iArg

                ' Set tooltip text to current function prototype
                If (tip.CurrentFunction = 0) Then
                    tip.TipText = "SomeFunc( Blah, blah, blah... )"
                Else
                    tip.TipText = "SomeFunc( Foobar )"
                End If

            End If

        End If

    End If

End Sub

Private Function CodeMax1_KeyPress(ByVal Control As CodeMaxCtl.ICodeMax, ByVal KeyAscii As Long, ByVal Shift As Long) As Boolean

    ' Invoke the CodeTip feature when the user presses the '(' key. We'll
    ' determine whether or not we are actually ready to handle the message
    ' in the OnCodeTip handler.
    If (Asc("(") = KeyAscii) Then
        Control.ExecuteCmd (cmCmdCodeTip)
    End If

End Function

Private Sub CodeMax1_RegisteredCmd(ByVal Control As CodeMaxCtl.ICodeMax, ByVal lCmd As CodeMaxCtl.cmCommand)
    ' get the current selection
    Dim r As New CodeMaxCtl.range
    Set r = Control.GetSel(False)

    If (lCmd = 1000) Then
        ' <DebugStepInto>
        'highlight the current line
        Control.HighlightedLine = r.EndLineNo
        ' <do whatever here>
    ElseIf lCmd = 1001 Then
        ' <DebugStepOver>
        ' highlight the current line
        Control.HighlightedLine = r.EndLineNo
        ' <do whatever here>
    ElseIf lCmd = 1002 Then
        ' <DebugBreakpoint>
        ' show the bookmark image in the left margin
        Dim lImages As Long
        Dim lTemp As Long
        lImages = Control.GetMarginImages(r.EndLineNo)
        lTemp = lImages And 2  '2 is the image bit in the imagelist
        If (lTemp <> 0) Then
            ' breakpoint is on -- turn it off
            lImages = lImages And Not (2)
        Else
            ' breakpoint is off -- turn it on
            lImages = lImages Or 2
        End If

        ' switch to the new image for the current line
        Call Control.SetMarginImages(r.EndLineNo, lImages)
    End If
End Sub

Private Sub Form_Load()
    ' Switch to the new images.  Note: The first image must always be the bookmark image.
    CodeMax1.ImageList = ImageList1

    ' register a new 'DebugStepInto' command
    Dim g As New CodeMaxCtl.Globals
    Call g.RegisterCommand(1000, "DebugStepInto", "Steps into the current function while debugging.")
    ' Map F8 to DebugStepInto
    Dim h As New CodeMaxCtl.HotKey
    h.VirtKey1 = Chr(&H77)  'F8
    Call g.RegisterHotKey(h, 1000)

    ' register a new 'DebugStepOver' command
    Call g.RegisterCommand(1001, "DebugStepOver", "Steps over the current function while debugging.")
    ' Map F10 to DebugStepOver
    h.VirtKey1 = Chr(&H79)  'F10
    Call g.RegisterHotKey(h, 1001)

    ' register a new 'DebugBreakpoint' command
    Call g.RegisterCommand(1002, "DebugBreakpoint", "Sets a breakpoint at the current line.")
    ' Map F9 to DebugBreakpoint
    h.VirtKey1 = Chr(&H78)  'F9
    Call g.RegisterHotKey(h, 1002)

    CodeMax1.Text = "This simple VB app demonstrates a few concepts:" + vbLf + vbLf
    CodeMax1.AddText ("1. custom commands:  F8, F9, and F10 are mapped to commands a debugger might require." + vbLf)
    CodeMax1.AddText ("2. Line highlighting.  F8 and F10 set the current highlighted line." + vbLf)
    CodeMax1.AddText ("3. custom margin images:  The bookmark image (Ctrl+F2), as well as a breakpoint (F9) image are defined." + vbLf)
    CodeMax1.AddText ("4. retrieving a macro's definition:  Not a new feature, but often asked about." + vbLf)
    CodeMax1.AddText ("5. Horizontal line dividers." + vbLf)

    ' add the horizontal line divider
    Call CodeMax1.SetDivider(2, True)

    ' clear the undo chain so the text inserted above won't be undoable
    Call CodeMax1.ClearUndoBuffer

    'here is how you retrieve all of the hotkeys at once:
    Dim hotkeys() As Byte
    Call g.GetHotKeys(hotkeys)

End Sub

Private Sub Form_Resize()
CodeMax1.Left = 0
CodeMax1.Top = 0
CodeMax1.Width = Form1.ScaleWidth
CodeMax1.Height = Form1.ScaleHeight
End Sub
