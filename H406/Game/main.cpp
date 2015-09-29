//==============================================================================
//
// main[main.cpp]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================

//==============================================================================
// Anonymous namespace
//------------------------------------------------------------------------------
namespace {
  // égÇÌÇ»Ç¢ïœêîóp
  template <class  _T>
  void inline UnusedParam(_T param) {
    (param);
  }
}

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
  UnusedParam(hInstance);
  UnusedParam(hPrevInstance);
  UnusedParam(lpCmdLine);
  UnusedParam(nCmdShow);
  App& app = App::instance(1280,720);
  app.setTitle("adad");
  return app.run();
}



//EOF