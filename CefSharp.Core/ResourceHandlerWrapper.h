﻿// Copyright © 2010-2015 The CefSharp Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "Stdafx.h"
#include "include/cef_scheme.h"
#include "Internals/AutoLock.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Specialized;

namespace CefSharp
{
    public class ResourceHandlerWrapper : public CefResourceHandler
    {
    private:
        gcroot<IRequest^> _request;
        gcroot<IResourceHandler^> _handler;
        gcroot<Stream^> _stream;
        gcroot<ICallback^> _callbackWrapper;
        gcroot<IBrowser^> _browser;
        gcroot<IFrame^> _frame;

        CriticalSection _syncRoot;
        int64 SizeFromStream();

    public:

        /// <summary>
        /// Constructor that accepts IBrowser, IFrame, IRequest in order to be the CefSharp
        /// lifetime management container  (i.e. calling .Dispose at the correct time) on 
        /// managed objects that contain MCefRefPtrs.
        /// </summary>
        ResourceHandlerWrapper(IResourceHandler^ handler, IBrowser ^browser, IFrame^ frame, IRequest^ request)
            : _handler(handler), _browser(browser), _frame(frame), _request(request)
        {
        }

        ResourceHandlerWrapper(IResourceHandler^ handler) 
            : _handler(handler)
        {
            if (static_cast<IResourceHandler^>(_handler) == nullptr)
            {
                throw gcnew ArgumentException("handler must not be null");
            }
        }

        ~ResourceHandlerWrapper()
        {
            _handler = nullptr;
            _stream = nullptr;
            delete _callbackWrapper;
            delete _request;
            delete _browser;
            delete _frame;
        }

        virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback);
        virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl);
        virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback);
        virtual void Cancel();

        IMPLEMENT_REFCOUNTING(ResourceHandlerWrapper);
    };
}