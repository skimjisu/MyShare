unit Unit2;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm2 = class(TForm)
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

{$R *.dfm}

procedure TForm2.Button1Click(Sender: TObject);
var
  Index: integer;
  SelectedItem : TListItem;
begin
   if ListView1.SelCount > 0 then
   begin
      Index := ListView1.Selected.Index;
      SelectedItem := ListView1.Selected;
      if Index < (ListView1.Items.Count - 1) then
      begin
         ListView1.Items.BeginUpdate;
         try
            ListView1.Items.Insert(Index+2).Assign(SelectedItem);
            ListView1.Items.Delete(Index);
            ListView1.Selected := ListView1.Items[Index+1];
         finally
            ListView1.Items.EndUpdate;
         end;
      end;
   end;
end;

end.
