using System;
using System.IO;
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
    public const string kURL = "ws://192.168.11.200:7682/led";

    public Image image = null;
    public Vector Pos = new Vector(0, 0);
    public Vector MoveVec = new Vector(1, 3);

    public Rectangle[] Gauge = new Rectangle[2];
    private float GaugeTimer = 0;
    private bool LeadIsShow = false;

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
        "LED_BBAwin.png",
        "LED_GGEwin.png",
        "white.bmp",
      };

      for (int i = 0; i < imageFile.Length; i++)
      {
        Uri uri = new Uri(AppDomain.CurrentDomain.BaseDirectory + imageFile[i], UriKind.Absolute);
        BitmapImages[i] = new BitmapImage(uri);
      }
      image.Source = BitmapImages[0];

      Gauge[0] = new Rectangle();
      Gauge[0].Width = 128;
      Gauge[0].Height = 32;
      Gauge[0].Fill = Brushes.Blue;
      this.LEDCanvas.Children.Add(Gauge[0]);
      Gauge[1] = new Rectangle();
      Gauge[1].Width = 128;
      Gauge[1].Height = 32;
      Gauge[1].Fill = Brushes.Yellow;
      Gauge[1].Margin = new Thickness(128 - Gauge[1].Width,0,0,0);
      this.LEDCanvas.Children.Add(Gauge[1]);

      GaugeTimer = 0;
    }

    void Update(object sender, EventArgs e){

      if (recvText != "")
      {
        MakeText(recvText, Brushes.White);
        recvText = "";
      }

      switch (recvData.events)
      {
        // ゲージを表示する
        case LedEvent.ShowGauge:
          if (recvData._s32 != -4194304)
          {
            Gauge[0].Width = 128 * recvData._f32 * GaugeTimer;
            Gauge[1].Width = 128 * (1.0f - recvData._f32) * GaugeTimer;
            GaugeTimer += (1 - GaugeTimer) * 0.05f;
            Gauge[1].Margin = new Thickness(128 - Gauge[1].Width, 0, 0, 0);
            LeadIsShow = false;
          }
        break;
        case LedEvent.ShowLead:
          if(!LeadIsShow){
            image.Source = BitmapImages[(int)recvData.events - recvData._s32];

            if(recvData._s32 == 0){
              MakeText("1Pリード中！！", Brushes.White);
            }
            else if (recvData._s32 == 1)
            {
              MakeText("2Pリード中！！", Brushes.White);
            }
            else
            {
              MakeText("接戦！！", Brushes.Black);
            }
            LeadIsShow = true;
            GaugeTimer = 0;
          }
        break;
        case LedEvent.ShowSec:
        if(!LeadIsShow){
          // 新しいテキストを生成
          TextBlock textBlock = new TextBlock();
          textBlock.FontSize = 20;
          textBlock.Text = "のこり" + recvData._s32 + "秒！！";
          textBlock.Foreground = Brushes.White;
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
          TranslateTransform transform = new TranslateTransform(0, verticalPosition);

          // テキストのアニメーション
          textBlock.RenderTransform = transform;
          Duration duration = new Duration(TimeSpan.FromMilliseconds(1000));
          DoubleAnimation animationX = new DoubleAnimation(0, duration);
          animationX.Completed += new EventHandler(animationX_Completed);
          animationX.Name = textBlock.Name;
          transform.BeginAnimation(TranslateTransform.XProperty, animationX);
          LeadIsShow = true;
          image.Source = BitmapImages[(int)recvData.events];
        }
        break;

        default:
          image.Source = BitmapImages[(int)recvData.events];
          GaugeTimer = 0;
          break;
      }

      // 非表示にする
      if (recvData.events != LedEvent.ShowGauge)
      {
        Gauge[0].Width = 0;
        Gauge[1].Width = 0;
      }
    }


    public void MakeText(string text, Brush fontcolor)
    {
      // フォントサイズの決定
      double fontsize = 20;
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
        LeadIsShow = false;
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
