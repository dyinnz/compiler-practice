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
    ScopeGuarder(ScopeGuarder&&) = default;
    ~ScopeGuarder() { _guard();  }

  private:
    std::function<void()> _guard;
    ScopeGuarder& operator=(ScopeGuarder&&) = delete;
    ScopeGuarder& operator=(const ScopeGuarder&) = delete;
    ScopeGuarder(const ScopeGuarder&) = delete;
};

#define ScopeGuard ScopeGuarder __FILE__##__LINE__##scope_guarder = (std::function<void()>)[&]()
