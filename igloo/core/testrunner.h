
//          Copyright Joakim Karlsson & Kim Gräsman 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_TESTRUNNER_H
#define IGLOO_TESTRUNNER_H

#include <list>
#include <igloo/core/testresult.h>

namespace igloo {
  class TestRunner
  {
  public:
    typedef std::pair<std::string, BaseContextRunner*> NamedContextRunner;
    typedef std::vector<NamedContextRunner> ContextRunners;
    
    static int RunAllTests()
    {
      std::list<TestResult> results;
      
      for (ContextRunners::iterator it = FixtureRunners().begin(); it != FixtureRunners().end(); it++)
      {
        (*it).second->Run((*it).first, results);
        delete (*it).second;
      }
      
      FixtureRunners().clear();
      
      std::cout << std::endl;
      
      return CheckResults(results);
    }
    
    static int CheckResults(const std::list<TestResult>& results)
    {
      int run = 0, succeeded = 0, failed = 0;
      
      for (std::list<TestResult>::const_iterator it = results.begin(); it != results.end(); it++)
      {
        run++;
        if ((*it).GetSuccess())
        {
          succeeded++;
        }
        else
        {
          failed++;
          
          const TestResult& result = *it;
          
          std::cout << result.GetContextName() << "::" << result.GetSpecName() << " failed:" << std::endl << result.GetErrorMessage() << std::endl;
        }
      }
      
      std::cout << "Test run complete. " << run << " tests run, " << succeeded << " succeeded, " << failed << " failed." << std::endl;
      return failed;
    }
    
    template <typename FixtureRunnerType>
    static void RegisterContext(const std::string& name)
    {
      if(!TestFixtureIsRegistered(name))
      {
        TestRunner::FixtureRunners().push_back(std::make_pair(name, new FixtureRunnerType()));
      }
    }
    
    static bool TestFixtureIsRegistered(const std::string& name)
    {
      for (ContextRunners::iterator it = FixtureRunners().begin(); it != FixtureRunners().end(); it++)
      {
        if((*it).first == name)
        {
          return true;
        }
      }      
      
      return false;
    }
    
    static TestRunner::ContextRunners& FixtureRunners()
    {
      static TestRunner::ContextRunners _fixtureRunners;
      return _fixtureRunners;
    }
  };
}

#endif // IGLOO_TESTRUNNER_H
