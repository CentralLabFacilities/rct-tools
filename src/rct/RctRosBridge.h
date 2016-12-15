/*
 * RctRosBridge.h
 *
 *  Created on: Dec 16, 2014
 *      Author: leon
 */

#pragma once
#include <string>
#include <vector>
#include <rct/impl/TransformCommunicator.h>
#include <boost/thread.hpp>
#include <rsc/logging/Logger.h>

namespace rct {

class RctRosBridge;

class Handler: public TransformListener {
public:
	typedef boost::shared_ptr<Handler> Ptr;
	Handler(RctRosBridge* parent, const std::string &loggerSuffix = "") :
			parent(parent),
			logger(
					rsc::logging::Logger::getLogger(
							std::string("rct.RctRosBridge.Handler") + loggerSuffix)) {
	}
	virtual ~Handler() {
	}
	void newTransformAvailable(const Transform& transform, bool isStatic);
	bool hasTransforms();
	TransformWrapper nextTransform();
private:
	RctRosBridge* parent;
	boost::mutex mutexHandler;
	std::vector<TransformWrapper> transforms;
	rsc::logging::LoggerPtr logger;
};

class RctRosBridge {
public:
	RctRosBridge(const std::string &name = "rct-ros-bridge", bool rosLegacyMode = true, long rosLegacyIntervalMSec = 100);
	virtual ~RctRosBridge();

	bool run();
	void interrupt();
	void notify();
private:
	TransformCommunicator::Ptr commRsb;
	TransformCommunicator::Ptr commRos;
	Handler::Ptr rosHandler;
	Handler::Ptr rsbHandler;
	bool interrupted;

	boost::condition_variable cond;
	boost::mutex mutex;

	static rsc::logging::LoggerPtr logger;
};

} /* namespace rct */
