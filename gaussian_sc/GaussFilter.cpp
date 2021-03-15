#include <cmath>

#include "GaussFilter.h"

GaussFilter::GaussFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

void GaussFilter::do_filter() {
  { wait(); }
  while (true) {
    unsigned int cnt = 0;
    r_val = 0; g_val = 0; b_val = 0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
// std::cout << "(u, v) = " << u << ", " << v << std::endl;        
        unsigned char r_value = i_r.read();
        unsigned char g_value = i_g.read();
        unsigned char b_value = i_b.read();
        wait();
// std::cout << int(r_value) << ", " << int(g_value) << ", " << int(b_value) << std::endl;    
        if (r_value != 0 && g_value != 0 && b_value !=0 ) cnt+=mask[u][v];
        r_val += r_value * mask[u][v];
        g_val += g_value * mask[u][v];
        b_val += b_value * mask[u][v];
// std::cout << "sum = " << r_val << ", " << g_val  << ", " << b_val << std::endl;    
        wait();
      }
    }
// std::cout << cnt << std::endl;    
    o_result_r.write(r_val / cnt);    
    o_result_g.write(g_val / cnt);
    o_result_b.write(b_val / cnt);
// std::cout << "sum = " << r_val << ", " << g_val  << ", " << b_val << std::endl;    
// std::cout << "sum = " << (r_val >> 4) << ", " << (g_val >> 4)  << ", " << (b_val >> 4) << std::endl;    
  }
}
