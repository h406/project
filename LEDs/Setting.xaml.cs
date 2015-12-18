using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace LEDs
{
  /// <summary>
  /// Setting.xaml の相互作用ロジック
  /// </summary>
  public partial class Setting : Window
  {
    public Setting()
    {
      InitializeComponent();
      this.URL.Text = MainWindow.URL;
    }

    private void Button_Click(object sender, RoutedEventArgs e)
    {
      MainWindow.URL = this.URL.Text;
      MainWindow window = new MainWindow();
      window.Show();
      this.Close();
    }
  }
}
