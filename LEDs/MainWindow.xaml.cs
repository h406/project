using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using WebSocket4Net;

namespace LEDs
{
  // シーン
  public enum LedEvent
  {
    NoneConect = 0,
    Conect,
    MoveTitle,
    MoveSelect,
    MoveGame,
    ShowGauge,
    ShowLead,
    ShowSec,
  };

  [StructLayout(LayoutKind.Explicit)]
  struct RecvData
  {
    [FieldOffset(0)] public LedEvent events;
    [FieldOffset(4)] public int _s32;
    [FieldOffset(4)] public float _f32;
  }

  public partial class MainWindow : Window
  {
    public const string kURL = "ws://localhost:7682/led";

    public Image image = null;
    public Vector Pos = new Vector(0, 0);
    public Vector MoveVec = new Vector(1, 3);

    private WebSocket ws = null;
    private Thread SockThread = null;
    private bool IsExit = false;

    private BitmapImage[] BitmapImages = new BitmapImage[Enum.GetNames(typeof(LedEvent)).Length];
    private RecvData recvData;
    private string recvText = "";

    public MainWindow()
    {
      InitializeComponent();
      image = new Image();
      image.Stretch = Stretch.Fill;
      image.Width = 128;
      image.Height = 32;
      this.LEDCanvas.Children.Add(image);

      SockThread = new Thread(SockUpdate);
      SockThread.Start();

      string[] imageFile = {
        "noneconnect.bmp",
        "connect.bmp",
        "title.bmp",
        "select.bmp",
        "game.bmp",
        "showGauge.bmp",
        "showLead.bmp",
        "showSec.bmp",
      };

      for (int i = 0; i < imageFile.Length; i++)
      {
        BitmapImages[i] = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + imageFile[i], UriKind.Absolute));
      }
      image.Source = BitmapImages[0];
    }

    void Update(object sender, EventArgs e){

      image.Source = BitmapImages[(int)recvData.events];

      if (recvText != "")
      {
        MakeText(recvText);
        recvText = "";
      }
      /*
      switch (recvData.events)
      {
        case LedEvent.NoneConect:
          image.Source = BitmapImages[0];
        break;
        case LedEvent.Conect:
          image.Source = BitmapImages[1];
        break;
      }
      */
    }

    public void MakeText(string text)
    {
      // フォントサイズの決定
      double fontsize = 20;
      // フォントカラーの決定
      Brush fontcolor = Brushes.White;
      // 移動速度
      int moveCommentTime = 5000;

      // 新しいテキストを生成
      TextBlock textBlock = new TextBlock();
      textBlock.FontSize = fontsize;
      textBlock.Text = text;
      textBlock.Foreground = fontcolor;
      this.TextCanvas.Children.Add(textBlock);

      // テキストに影をつける
      DropShadowEffect effect = new DropShadowEffect();
      effect.ShadowDepth = 4;
      effect.Direction = 330;
      effect.Color = (Color)ColorConverter.ConvertFromString("black");
      textBlock.Effect = effect;

      // テキストの位置を指定
      // verticalPosition += (int)fontsize;
      // if (verticalPosition + (int)fontsize >= this.Height) verticalPosition = 0;
      float verticalPosition = 0;
      TranslateTransform transform = new TranslateTransform(this.Width, verticalPosition);

      // テキストのアニメーション
      textBlock.RenderTransform = transform;
      Duration duration = new Duration(TimeSpan.FromMilliseconds(moveCommentTime));
      DoubleAnimation animationX = new DoubleAnimation(-1 * text.Length * fontsize, duration);
      animationX.Completed += new EventHandler(animationX_Completed);
      animationX.Name = textBlock.Name;
      transform.BeginAnimation(TranslateTransform.XProperty, animationX);
    }

    private void animationX_Completed(object sender, EventArgs e)
    {
      AnimationClock clock = (AnimationClock)sender;
      lock (this.TextCanvas.Children)
      {
        for (int i = 0; i < this.TextCanvas.Children.Count; i++)
        {
          if (((TextBlock)this.TextCanvas.Children[i]).Name.Equals(clock.Timeline.Name))
          {
            this.TextCanvas.Children.RemoveAt(i);
            break;
          }
        }
      }
    }

    private void SockUpdate()
    {
      ws = new WebSocket(kURL);

      /// 文字列受信
      ws.MessageReceived += (s, e) =>
      {
        // Console.WriteLine("{0}:String Received:{1}", DateTime.Now.ToString(), e.Message);
        recvText = (e.Message);
      };

      /// バイナリ受信
      ws.DataReceived += (s, e) =>
      {
        IntPtr ptr = Marshal.UnsafeAddrOfPinnedArrayElement(e.Data, 0);
        recvData = (RecvData)Marshal.PtrToStructure(ptr, typeof(RecvData));
        string test = recvData.ToString();
      };

      /// サーバ接続完了
      ws.Opened += (s, e) =>
      {
        recvData.events = LedEvent.Conect;
      };

      // 接続切れた
      ws.Closed += (s, e) =>
      {
        recvData.events = LedEvent.NoneConect;
      };

      /// サーバ接続開始
      ws.Open();

      /// 送受信ループ
      while (!IsExit)
      {
        // 空いた
        if (ws.State == WebSocketState.Open)
        {

        }

        // コネクトが出来なけれなもう一度試す
        if (ws.State == WebSocketState.Closed)
        {
          // 1秒スリーブ
          Thread.Sleep(1000);
          ws.Open();
        }
      }

      /// ソケットを閉じる
      ws.Close();
    }

    private void Window_KeyDown(object sender, KeyEventArgs e)
    {
      if (e.Key == Key.Escape)
      {// どのボタンを選択したかを判断する
        if (MessageBox.Show("終了しますか", "確認", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
        {
          this.Close();
        }
      }
    }

    private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
    {
      IsExit = true;
      SockThread.Join();
    }

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      //イベント登録
      CompositionTarget.Rendering += new EventHandler(Update);
    }
  }
}
