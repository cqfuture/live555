#ifndef _MEDIA_HH
#define _MEDIA_HH

#ifndef _LIVEMEDIA_VERSION_HH
#include "liveMedia_version.hh"
#endif

#ifndef _HASH_TABLE_HH
#include "HashTable.hh"
#endif

#ifndef _USAGE_ENVIRONMENT_HH
#include "UsageEnvironment.hh"
#endif

// Lots of files end up needing the following, so just #include them here:
#ifndef _NET_COMMON_H
#include "NetCommon.h"
#endif

#include <stdio.h>

// The following makes the Borland compiler happy:
#ifdef __BORLANDC__
#define _strnicmp strnicmp
#define fabsf(x) fabs(x)
#endif

#define mediumNameMaxLen 30

class Medium 
{
public:
	static Boolean lookupByName(
						UsageEnvironment& env,
						char const* mediumName,
						Medium*& resultMedium );
	
	static void close(
					UsageEnvironment& env, 
					char const* mediumName );

	// alternative close() method using ptrs
	// (has no effect if medium == NULL)
	static void close( Medium* medium ); 
	
	UsageEnvironment& envir() const 
	{
		return fEnviron;
	}
	
	char const* name() const 
	{
		return fMediumName;
	}
	
	// Test for specific types of media:
	virtual Boolean isSource() const;
	
	virtual Boolean isSink() const;
	
	virtual Boolean isRTCPInstance() const;
	
	virtual Boolean isRTSPClient() const;
	
	virtual Boolean isRTSPServer() const;
	
	virtual Boolean isMediaSession() const;
	
	virtual Boolean isServerMediaSession() const;
	
	virtual Boolean isDarwinInjector() const;

protected:
	
	friend class MediaLookupTable;
	
	Medium( UsageEnvironment& env ); // abstract base class
	
	virtual ~Medium(); // instances are deleted using close() only
	
	TaskToken& nextTask() 
	{
		return fNextTask;
	}

private:
	UsageEnvironment& fEnviron;
	
	char fMediumName[mediumNameMaxLen];
	
	TaskToken fNextTask;
};

// A data structure for looking up a Medium by its string name.
// (It is used only to implement "Medium", but we make it visible here, 
// in case developers want to use it to iterate over
// the whole set of "Medium" objects that we've created.)
class MediaLookupTable
{
public:
	static MediaLookupTable* ourMedia( UsageEnvironment& env );
	
	HashTable const& getTable() 
	{ 
		return *fTable; 
	}

protected:
	MediaLookupTable(UsageEnvironment& env);
	
	virtual ~MediaLookupTable();

private:
	friend class Medium;

	// Returns NULL if none already exists
	Medium* lookup(char const* name) const;
	
	void addNew(Medium* medium, char* mediumName);
	
	void remove(char const* name);
	
	void generateNewName(char* mediumName, unsigned maxLen);

private:
  
	UsageEnvironment& fEnv;
  
	HashTable* fTable;
  
	unsigned fNameGenerator;
};


// The structure pointed to by the "liveMediaPriv" UsageEnvironment field:
class _Tables 
{

public:
	// returns a pointer to an "ourTables" structure (creating it if necessary)
	static _Tables* getOurTables(
								UsageEnvironment& env, 
								Boolean createIfNotPresent = True );
	
	// used to delete ourselves when we're no longer used
	void reclaimIfPossible();
	
	MediaLookupTable* mediaTable;
	
	void* socketTable;

protected:
	
	_Tables( UsageEnvironment& env );
	
	virtual ~_Tables();

private:
	
	UsageEnvironment& fEnv;

};

#endif
