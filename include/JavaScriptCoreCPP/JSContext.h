// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSContext_h_
#define _Titanium_Mobile_JSContext_h_

#include "JavaScriptCoreCPP/JSVirtualMachine.h"
#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <atomic>
#include <functional>

class JSValue;
using JSValue_ptr_t = std::shared_ptr<JSValue>;

class JSContext;
using JSContext_ptr_t = std::shared_ptr<JSContext>;

/*!
  @interface
  @discussion An instance of JSContext represents a JavaScript execution environment. All
  JavaScript execution takes place within a context.
  JSContext is also used to manage the life-cycle of objects within the
  JavaScript virtual machine. Every instance of JSValue is associated with a
  JSContext via a strong reference. The JSValue will keep the JSContext it
  references alive so long as the JSValue remains alive. When all of the JSValues
  that reference a particular JSContext have been deallocated the JSContext
  will be deallocated unless it has been previously retained.
*/
class JSContext final : public std::enable_shared_from_this<JSContext> {

public:
    
  // Factory function for creating a smart pointer to a JSContext. This is the
  // public API to create a JSContext.
  template <typename... Ts>
      static JSContext_ptr_t create(Ts&&... params) {
    return JSContext_ptr_t(new JSContext(std::forward<Ts>(params)...), deleter{});
  }

  /*!
    @methodgroup Evaluating Scripts
  */
  /*!
    @method
    @abstract Evaluate a string of JavaScript code.
    @param script A string containing the JavaScript code to evaluate.
    @result The last value generated by the script.
  */
  JSValue_ptr_t evaluateScript(const std::string& script);
    
  /*!
    @property
    @abstract Get the global object of the context.
    @discussion This method retrieves the global object of the JavaScript execution context.
    @result The global object.
  */
  JSValue_ptr_t globalObject();
    
    
  /*!
    @property
    @discussion The <code>exception</code> property may be used to throw an exception to JavaScript.
     
    Before a callback is made from JavaScript to an C++ lambda or method,
    the prior value of the exception property will be preserved and the property
    will be set to nullptr. After the callback has completed the new value of the
    exception property will be read, and prior value restored. If the new value
    of exception is not nullptr, the callback will result in that value being thrown.
     
    This property may also be used to check for uncaught exceptions arising from
    API function calls (since the default behaviour of <code>exceptionHandler</code> is to
    assign an uncaught exception to this property).
     
    If a JSValue originating from a different JSVirtualMachine than this context
    is assigned to this property, an C++ exception will be thrown.
  */
  void setException(const JSValue_ptr_t& exception_ptr) {
    exception_ptr_ = exception_ptr;
  };
    
  JSValue_ptr_t exception() const {
    return JSValue_ptr_t(exception_ptr_.lock());
  };

  /*!
    @property
    @discussion If a call to an API function results in an uncaught JavaScript exception, the
    <code>exceptionHandler</code> block will be invoked. The default implementation for the
    exception handler will store the exception to the exception property on
    context. As a consequence the default behaviour is for unhandled exceptions
    occurring within a callback from JavaScript to be rethrown upon return.
    Setting this value to nullptr will result in all uncaught exceptions thrown from
    the API being silently consumed.
  */
  typedef std::function<void(const JSContext_ptr_t&, const JSValue_ptr_t&)> exceptionHandler_t;
    
  void setExceptionHandler(const exceptionHandler_t& exceptionHandler) {
    exceptionHandler_ = exceptionHandler;
  };
    
  exceptionHandler_t exceptionHandler() const {
    return exceptionHandler_;
  };
    
  /*!
    @property
    @discussion All instances of JSContext are associated with a single JSVirtualMachine. The
    virtual machine provides an "object space" or set of execution resources.
  */
  JSVirtualMachine_ptr_t virtualMachine() const {
    return virtualMachine_ptr_;
  }
    
  // Implicit conversion to C API.
  explicit operator ::JSGlobalContextRef() const {
    return context_;
  }
    
  static long ctorCounter() {
    return ctorCounter_;
  }
    
  static long dtorCounter() {
    return dtorCounter_;
  }
    
private:

  JSContext();

  JSContext(const JSVirtualMachine_ptr_t& virtualMachine_ptr);
    
  ~JSContext();
    
  // Create a copy of another JSContext.
  JSContext(const JSContext& rhs);
    
  // Create a copy of another JSContext by assignment.
  JSContext& operator=(const JSContext& rhs);
    
private:
    
  friend class JSValue;
    
  // This struct only exists so that a custom deleter can be passed to
  // std::shared_ptr<JSContext> while keeping the JSContext destructor
  // private.
  struct deleter {
    void operator()(JSContext* ptr) {
      delete ptr;
    }
  };
    
  JSValue_ptr_t valueFromNotifyException(::JSValueRef exceptionValue);
    
  void notifyException(::JSValueRef value);
    
  JSVirtualMachine_ptr_t virtualMachine_ptr_ { nullptr };
  ::JSGlobalContextRef   context_            { nullptr };
    
  // exception_ptr_ must be a weak pointer becuase we don't want to create a
  // cyclic dependency where a JSContext holds a strong reference to a
  // JSValue.
  std::weak_ptr<JSValue> exception_ptr_;
  exceptionHandler_t     exceptionHandler_ {[](const JSContext_ptr_t& context_ptr, const JSValue_ptr_t& exception_ptr) {context_ptr->setException(exception_ptr);}};
    
  static std::atomic<long> ctorCounter_;
  static std::atomic<long> dtorCounter_;
};

#endif // _Titanium_Mobile_JSContext_h_
