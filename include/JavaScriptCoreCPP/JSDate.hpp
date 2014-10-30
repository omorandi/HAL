/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSDATE_HPP_
#define _JAVASCRIPTCORECPP_JSDATE_HPP_

#include "JavaScriptCoreCPP/JSObject.hpp"

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript object of the Date type.

  The only way to create a JSDate is by using the
  JSContext::CreateDate member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSDate final : public JSObject, public detail::JSPerformanceCounter<JSDate> {
#else
class JSDate final : public JSObject {
#endif
	
 private:

	JSDate(const JSContext& js_context) : JSDate(js_context, std::vector<JSValue>()) {
	}
	
	JSDate(const JSContext& js_context, const std::vector<JSValue>& arguments);

	// Only a JSContext can create a JSDate.
	friend JSContext;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSDATE_HPP_