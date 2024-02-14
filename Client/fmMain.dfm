object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'MyShere'
  ClientHeight = 481
  ClientWidth = 582
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 40
    Width = 582
    Height = 129
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object LB_ReadPerSec: TLabel
      Left = 319
      Top = 51
      Width = 3
      Height = 13
    end
    object LB_TotalProce: TLabel
      Left = 76
      Top = 51
      Width = 3
      Height = 13
    end
    object LB_TotalPercent: TLabel
      Left = 524
      Top = 16
      Width = 23
      Height = 13
      Caption = ' 0%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LB_RemainTime: TLabel
      Left = 319
      Top = 76
      Width = 3
      Height = 13
    end
    object LB_TotalSize: TLabel
      Left = 76
      Top = 76
      Width = 3
      Height = 13
    end
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 26
      Height = 13
      Caption = #50756#47308
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 12
      Top = 51
      Width = 61
      Height = 13
      Caption = #51204#52404#54028#51068' : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 12
      Top = 76
      Width = 61
      Height = 13
      Caption = #51204#52404#53356#44592' : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 252
      Top = 51
      Width = 61
      Height = 13
      Caption = #51069#44592#49549#46020' : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 252
      Top = 76
      Width = 61
      Height = 13
      Caption = #45224#51008#49884#44036' : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 12
      Top = 105
      Width = 64
      Height = 13
      Caption = #51089#50629' '#54028#51068' : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LB_NowFile: TLabel
      Left = 82
      Top = 105
      Width = 490
      Height = 13
      AutoSize = False
      EllipsisPosition = epPathEllipsis
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object PB_WorkerProce: TProgressBar
      Left = 41
      Top = 16
      Width = 478
      Height = 11
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 169
    Width = 582
    Height = 268
    BevelOuter = bvNone
    Color = clGradientActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object LV_ItemList: TListView
      Left = 0
      Top = 0
      Width = 582
      Height = 268
      Align = alClient
      BevelInner = bvNone
      BorderStyle = bsNone
      Columns = <>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      GridLines = True
      HideSelection = False
      MultiSelect = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 582
    Height = 40
    Align = alTop
    BevelOuter = bvNone
    Color = clGradientActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
  end
  object Panel4: TPanel
    Left = 0
    Top = 438
    Width = 582
    Height = 43
    Align = alBottom
    TabOrder = 3
    object Btn_Start: TButton
      Left = 499
      Top = 8
      Width = 75
      Height = 25
      Caption = #49884#51089
      TabOrder = 0
      OnClick = Btn_StartClick
    end
    object Btn_ClearItem: TButton
      Left = 111
      Top = 8
      Width = 75
      Height = 25
      Caption = #51648#50864#44592
      TabOrder = 1
      OnClick = Btn_ClearItemClick
    end
    object Btn_ChoiceFile: TButton
      Left = 8
      Top = 8
      Width = 73
      Height = 25
      Caption = #54028#51068#49440#53469
      TabOrder = 2
      OnClick = Btn_ChoiceFileClick
    end
  end
  object Dlg_ChoiceFile: TOpenDialog
    Options = [ofReadOnly, ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 112
    Top = 9
  end
  object FileWorkerTimer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = FileWorkerTimerTimer
    Left = 168
    Top = 9
  end
  object ReadPerSecTimer: TTimer
    Enabled = False
    OnTimer = ReadPerSecTimerTimer
    Left = 216
    Top = 9
  end
  object TotalWorkerTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TotalWorkerTimerTimer
    Left = 264
    Top = 9
  end
end
