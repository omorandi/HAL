//
//  JSStringTests.m
//  TiValue
//
//  Created by Matt Langston on 9/10/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JavaScriptCoreCPP/JSString.h"
#include <iostream>
#include <codecvt>

@interface JSStringTests : XCTestCase
@end

@implementation JSStringTests

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each test method in the class.
  [super tearDown];

  std::cout << "MDL: JSString::ctorCounter = " << JSString::ctorCounter() << std::endl;
  std::cout << "MDL: JSString::dtorCounter = " << JSString::dtorCounter() << std::endl;
}

- (void)testEmptyString {
  JSString string;
  XCTAssertEqual(0, string.size());
    
  std::string stdString = static_cast<std::string>(string);
  XCTAssertEqual(0, stdString.size());
  XCTAssertTrue(stdString.empty());
    
}

- (void)testEqual {
  JSString string1 { "hello, world" };
  JSString string2 = string1;
  XCTAssertEqual(string1, string2);
    
  JSString string3 { "hello" };
  XCTAssertNotEqual(string1, string3);
}

- (void)testStdString {
  JSString string1 { "hello, JSString" };
  XCTAssertEqual("hello, JSString", static_cast<std::string>(string1));
    
  std::string string2 { "hello, std::string" };
  XCTAssertEqual("hello, std::string", static_cast<std::string>(JSString(string2)));
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testPerformanceExample {
  // This is an example of a performance test case.
  [self measureBlock:^{
      // Put the code you want to measure the time of here.
    }];
}
#endif

@end
