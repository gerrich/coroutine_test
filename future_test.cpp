#include <boost/thread/future.hpp>
#include <boost/shared_ptr.hpp>

#define future unique_future
int main() {
  boost::promise<int> pi;
  boost::future<int> fi;
  fi = pi.get_future();

  pi.set_value(42);
  //boost::thread task(boost::move(pt)); // launch task on a thread

  //fi.wait(); // wait for it to finish

  assert(fi.is_ready());
  assert(fi.has_value());
  assert(!fi.has_exception());
  assert(fi.get_state()==boost::future_state::ready);
  assert(fi.get()==42);

  return 0;
}
