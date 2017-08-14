
unit ufrmSerialPack;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ComCtrls, LazSerial;


type
  { TfrmSerialPack }

  TfrmSerialPack = class(TForm)
    cmdOpenPort: TButton;
    cmdConfig: TButton;
    cmdEnviar: TButton;
    cmdExit: TButton;
    cmdClosePort: TButton;
    pacHex: TEdit;
    txtTfunc: TEdit;
    txtPack: TEdit;
    GroupBox1: TGroupBox;
    Label2: TLabel;
    txtDev1: TEdit;
    txtDev2: TEdit;
    txtPort: TEdit;
    txtValor: TEdit;
    txtEjej: TEdit;
    Label1: TLabel;
    Serial: TLazSerial;
    procedure cmdClosePortClick(Sender: TObject);
    procedure cmdConfigClick(Sender: TObject);
    procedure cmdEnviarClick(Sender: TObject);
    procedure cmdExitClick(Sender: TObject);
    procedure cmdOpenPortClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure SerialRxData(Sender: TObject);
    procedure txtEjejChange(Sender: TObject);
    procedure txtPackChange(Sender: TObject);
    procedure txtValorChange(Sender: TObject);
  private
    procedure generatePack() ;
    { private declarations }
  public
    { public declarations }
  end;


const
  Start_Char : Byte  = $7B;
  End_Char : Byte  = $7D;

var
  frmSerialPack: TfrmSerialPack;
  buff: String;
implementation
(*
typedef union {
 float float_valor;
 uint32_t uint32val;
 uint8_t binary[4];
} binary4;
  *)
  type
    TConverterRec = record
    case Boolean of
      false: (ByteArray: array[0..3] of Byte);
      true: (FloatValue: Single);
  end;
{$R *.lfm}

{ TfrmSerialPack }

procedure TfrmSerialPack.SerialRxData(Sender: TObject);
begin

end;

procedure TfrmSerialPack.txtEjejChange(Sender: TObject);
begin
  generatePack()
end;

procedure TfrmSerialPack.txtPackChange(Sender: TObject);
begin

end;

procedure TfrmSerialPack.txtValorChange(Sender: TObject);
begin
 generatePack();
end;

procedure TfrmSerialPack.FormCreate(Sender: TObject);
begin

end;

procedure TfrmSerialPack.cmdConfigClick(Sender: TObject);
begin
   Serial.ShowSetupDialog;
   txtPort.Text := Serial.Device;
end;

procedure TfrmSerialPack.cmdEnviarClick(Sender: TObject);
var
  tstr: String;
begin
  tstr:=txtPack.Text;
  Serial.WriteData(buff);

end;

procedure TfrmSerialPack.cmdClosePortClick(Sender: TObject);
begin
   Serial.Close;
end;

procedure TfrmSerialPack.cmdExitClick(Sender: TObject);
begin
  Application.Terminate;
end;

procedure TfrmSerialPack.cmdOpenPortClick(Sender: TObject);
begin
  Serial.Device := txtPort.Text;
  Serial.Open;
end;

procedure TfrmSerialPack.generatePack();
var
  checksum : Byte;
  tmpByteD1 : Byte;
  tmpByteD2 : Byte;
  tmpByteTF : Byte;
  valor : Single;
  valorC : TConverterRec;
  byte1 : Byte;
  byte2 : Byte;
  byte3 : Byte;
  byte4 : Byte;
  tmpByteNF : Byte;


  tmpHex: String;
begin
     buff:='';
     buff+=chr(Start_Char);
     tmpByteD1:=byte(StrToInt('$'+ txtDev1.Text));
     buff+=chr(tmpByteD1);
     tmpByteD2:=byte(StrToInt('$'+ txtDev2.Text));
     buff+=chr(tmpByteD2);
     tmpByteTF:=byte(StrToInt('$'+ txtTfunc.Text));
     buff+=chr(tmpByteTF);
     if txtValor.Text='' then
     begin
        valor := 0 ;
     end
     else
         valor := StrToFloat(txtValor.Text);

     valorC.FloatValue:=valor;

     byte1:=valorC.ByteArray[3];
     byte2:=valorC.ByteArray[2];
     byte3:=valorC.ByteArray[1];
     byte4:=valorC.ByteArray[0];
     if txtEjej.Text='' then
       begin
           tmpByteNF := 16;
       end
     else
         tmpByteNF := 16+ byte(strtoint(txtEjej.Text));

     checksum:= $FF - byte(tmpByteD1+tmpByteD2+tmpByteTF+tmpByteNF+ byte1+ byte2+ byte3+ byte4);

     buff+=chr(tmpByteNF);
     buff+=chr(Byte1);
     buff+=chr(Byte2);
     buff+=chr(Byte3);
     buff+=chr(Byte4);
     buff+=chr(checksum);
     buff+=chr(End_Char);
     tmpHex:=hexStr(Start_Char,2)+ ' ';
     tmpHex+=hexStr(tmpByteD1,2)+ ' ';
     tmpHex+=hexStr(tmpByteD2,2)+ ' ';
     tmpHex+=hexStr(tmpByteTF,2)+ ' ';
     tmpHex+=hexStr(tmpByteNF,2)+ ' ';
     tmpHex+=hexStr(byte1,2)+' ' + hexStr(byte2,2)+' '+hexStr(byte3,2)+' '+hexStr(byte4,2)+' ';
     tmpHex+=hexStr(checksum,2)+ ' ';
     tmpHex+=hexStr(End_Char,2);

     pacHex.Text:=tmpHex ;
     txtPack.Text:=buff;
end;

end.

