#include <iostream>

#include <TString.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH2F.h>

TF1 * draw_line(unsigned int l_index, TString x, TString  y);

void hough_example()
{
  TCanvas mycan("c_line","",600,600);
  TH2F h_axes("h_axes","",100,-5,5,100,-5,5);
  h_axes.SetXTitle("m");
  h_axes.SetYTitle("q");
  h_axes.Draw();

  
  TF1 * f_line1 = draw_line(1,"0","1");
  f_line1->Draw("same");
  mycan.Print("c_line1.pdf");

  TF1 * f_line2 = draw_line(2,"1","3");
  f_line2->Draw("same");
  mycan.Print("c_line2.pdf");

  TF1 * f_line3 = draw_line(3,"2","3");
  f_line3->Draw("same");
  mycan.Print("c_line3.pdf");

  TF1 * f_line4 = draw_line(4,"0","4");
  f_line4->Draw("same");
  mycan.Print("c_line4.pdf");

  TF1 * f_line5 = draw_line(5,"1","2");
  f_line5->Draw("same");
  mycan.Print("c_line5.pdf");

  TF1 * f_line6 = draw_line(6,"2","2");
  f_line6->Draw("same");
  mycan.Print("c_line6.pdf");

  TF1 * f_line7 = draw_line(7,"1","0");
  f_line7->Draw("same");
  mycan.Print("c_line7.pdf");

}

TF1 * draw_line(unsigned int l_index, TString x_c, TString  y_c)
{
  TString x = x_c;
  TString y = y_c;
  TString s_index = TString::Format("%d", l_index); 
  TF1 * retval = new TF1("f_line" + s_index, y + " - " + x + "*x",-5,5);
  retval->SetLineColor(l_index);
  retval->Draw("same");
  return retval;
}
