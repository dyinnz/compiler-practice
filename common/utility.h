/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-05-27
 ******************************************************************************/

#pragma once

#include <functional>

class ScopeGuarder {
public:
  ScopeGuarder(std::function<void()> &&guard) : _guard(guard) {}

  ScopeGuarder(ScopeGuarder &&) = default;

  ~ScopeGuarder() { _guard(); }

private:
  std::function<void()> _guard;

  ScopeGuarder &operator=(ScopeGuarder &&) = delete;

  ScopeGuarder &operator=(const ScopeGuarder &) = delete;

  ScopeGuarder(const ScopeGuarder &) = delete;
};

#define ScopeGuard ScopeGuarder __FILE__##__LINE__##scope_guarder = \
(std::function<void()>)[&]()


template<class C, class F>
class filter_wrapper {
public:
  filter_wrapper(C container, F filter) : _container(container),
                                          _filter(filter) {}

  class filter_iterator {
  public:
    typedef typename C::const_iterator inner_iterator;

    filter_iterator(inner_iterator curr, inner_iterator end, F filter) :
        _curr(curr), _end(end), _filter(filter) {}

    filter_iterator &operator++() {
      if (_curr != _end) {
        for (++_curr; _curr != _end; ++_curr) {
          if (_filter(*_curr)) {
            return *this;
          }
        }
      }
      return *this;
    }

    typename C::const_reference operator*() {
      return *_curr;
    }

    bool operator!=(const filter_iterator &rhs) {
      return _curr != rhs._curr;
    }

  private:
    inner_iterator _curr;
    inner_iterator _end;
    F _filter;
  };

  typedef const filter_iterator const_iterator;
  typedef const typename C::const_reference const_reference;

  const_iterator begin() {
    for (auto beg = _container.begin(); beg != _container.end(); ++beg) {
      if (_filter(*beg)) {
        return const_iterator(beg, _container.end(), _filter);
      }
    }
    return end();
  }

  const_iterator end() {
    static const_iterator end(_container.end(), _container.end(), _filter);
    return end;
  }

private:
  C _container;
  F _filter;
};

template<class C, class F>
filter_wrapper<C, F> filter_container(C container, F filter) {
  return filter_wrapper<C, F>(container, filter);
};

