unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, VaConfigDialog, VaClasses, VaComm,
  Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    GroupBox2: TGroupBox;
    Label7: TLabel;
    Label8: TLabel;
    Label6: TLabel;
    VaComm1: TVaComm;
    VaConfigDialog1: TVaConfigDialog;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button5Click(Sender: TObject);
    procedure VaComm1RxBuf(Sender: TObject; Data: PVaData; Count: Integer);
    procedure Button1Click(Sender: TObject);
    procedure VaComm1RxChar(Sender: TObject; Count: Integer);
    procedure Button2Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  Settings:array[1..11] of byte = (83,16,7,208,1,232,1,232,10,163,31);
  SOS:array[1..9] of byte = (3,143,1,2,3,4,1,155,246);
  ReadSend:array[1..3] of byte;
  ReadBuf:array[1..905] of byte;
  AccelGyro:integer;
implementation

{$R *.dfm}


procedure TForm1.Button1Click(Sender: TObject);
begin
ReadSend[1] := 82;
ReadSend[2] := 1;
VaComm1.WriteBuf(ReadSend,2);
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
ReadSend[1] := 82;
ReadSend[2] := 2;
VaComm1.WriteBuf(ReadSend,2);
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
AccelGyro := 1;
ReadSend[1] := 82;
ReadSend[2] := 4;
VaComm1.WriteBuf(ReadSend,2)
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
AccelGyro := 1;
ReadSend[1] := 82;
ReadSend[2] := 3;
VaComm1.WriteBuf(ReadSend,2)
end;

procedure TForm1.Button5Click(Sender: TObject);
begin
VaComm1.WriteBuf(Settings,11);
end;

procedure TForm1.Button6Click(Sender: TObject);
begin
  VaComm1.WriteBuf(SOS,9);
end;

procedure TForm1.Button7Click(Sender: TObject);
begin
ReadSend[1] := 71;
ReadSend[2] := 79;
ReadSend[3] := 75;
VaComm1.WriteBuf(ReadSend,3)
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
VaComm1.Close;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
VaConfigDialog1.Execute;
VaComm1.Open;
end;

procedure TForm1.VaComm1RxBuf(Sender: TObject; Data: PVaData; Count: Integer);
begin
VaComm1.ReadBuf(ReadBuf, count);
end;

procedure TForm1.VaComm1RxChar(Sender: TObject; Count: Integer);
begin
if(AccelGyro <> 0) then
  VaComm1.ReadBuf(ReadBuf, 904)
else
VaComm1.ReadBuf(ReadBuf, 11);
if(ReadBuf[1] = 83) then
Begin
  Label9.Caption := IntToStr(ReadBuf[2]);
  Label10.Caption := IntToStr(ReadBuf[3]*256 + ReadBuf[4]);
  Label11.Caption := IntToStr(ReadBuf[5]*256 + ReadBuf[6]);
  Label12.Caption := IntToStr(ReadBuf[7]*256 + ReadBuf[8]);
  Label13.Caption := IntToStr(ReadBuf[9]);
End;

if(ReadBuf[1] = 67) then
Begin
if(ReadBuf[2] = 1) then
begin
  Label14.Caption := '���';
  Label15.Caption := '���';
  Label16.Caption := '��';
end;

End;
if(ReadBuf[1] = 71) then
begin
  Label17.Caption := '�������� ��������';
  AccelGyro := 0;
end;

if(ReadBuf[1] = 65) then
begin
  Label17.Caption := '������������ ��������';
  AccelGyro := 0;
end;


end;
end.
