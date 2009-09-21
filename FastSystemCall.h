#pragma once
#include "defs.h"

NTSTATUS FastSystemCall(size_t _RequestID, ...);

#define _NtAcceptConnectPort(PortHandle, PortIdentifier, Message, Accept, ServerView, ClientView) \
		FastSystemCall(0, 0, PortHandle, PortIdentifier, Message, Accept, ServerView, ClientView)

#define _NtAccessCheck(SecurityDescriptor, TokenHandle, DesiredAccess, GenericMapping, PrivilegeSet, PrivilegeSetLength, GrantedAccess, AccessStatus) \
		FastSystemCall(1, 0, SecurityDescriptor, TokenHandle, DesiredAccess, GenericMapping, PrivilegeSet, PrivilegeSetLength, GrantedAccess, AccessStatus)

#define _NtAccessCheckAndAuditAlarm(SubsystemName, HandleId, ObjectTypeName, ObjectName, SecurityDescriptor, DesiredAccess, GenericMapping, ObjectCreation, GrantedAccess, AccessStatus, GenerateOnClose) \
		FastSystemCall(2, 0, SubsystemName, HandleId, ObjectTypeName, ObjectName, SecurityDescriptor, DesiredAccess, GenericMapping, ObjectCreation, GrantedAccess, AccessStatus, GenerateOnClose)

#define _NtAddAtom(String, StringLength, Atom) \
		FastSystemCall(3, 0, String, StringLength, Atom)

#define _NtAdjustGroupsToken(TokenHandle, ResetToDefault, NewState, BufferLength, PreviousState, ReturnLength) \
		FastSystemCall(4, 0, TokenHandle, ResetToDefault, NewState, BufferLength, PreviousState, ReturnLength)

#define _NtAdjustPrivilegesToken(TokenHandle, DisableAllPrivileges, NewState, BufferLength, PreviousState, ReturnLength) \
		FastSystemCall(5, 0, TokenHandle, DisableAllPrivileges, NewState, BufferLength, PreviousState, ReturnLength)

#define _NtAlertResumeThread(ThreadHandle, PreviousSuspendCount) \
		FastSystemCall(6, 0, ThreadHandle, PreviousSuspendCount)

#define _NtAlertThread(ThreadHandle) \
		FastSystemCall(7, 0, ThreadHandle)

#define _NtAllocateLocallyUniqueId(Luid) \
		FastSystemCall(8, 0, Luid)

#define _NtAllocateUuids(UuidLastTimeAllocated, UuidDeltaTime, UuidSequenceNumber, UuidSeed) \
		FastSystemCall(9, 0, UuidLastTimeAllocated, UuidDeltaTime, UuidSequenceNumber, UuidSeed)

#define _NtAllocateVirtualMemory(ProcessHandle, BaseAddress, ZeroBits, AllocationSize, AllocationType, Protect) \
		FastSystemCall(10, 0, ProcessHandle, BaseAddress, ZeroBits, AllocationSize, AllocationType, Protect)

#define _NtCallbackReturn(Result, ResultLength, Status) \
		FastSystemCall(11, 0, Result, ResultLength, Status)

#define _NtCancelIoFile(FileHandle, IoStatusBlock) \
		FastSystemCall(12, 0, FileHandle, IoStatusBlock)

#define _NtCancelTimer(TimerHandle, PreviousState) \
		FastSystemCall(13, 0, TimerHandle, PreviousState)

#define _NtClearEvent(EventHandle) \
		FastSystemCall(14, 0, EventHandle)

#define _NtClose(Handle) \
		FastSystemCall(15, 0, Handle)

#define _NtCloseObjectAuditAlarm(SubsystemName, Id, GenerateOnClose) \
		FastSystemCall(16, 0, SubsystemName, Id, GenerateOnClose)

#define _NtCompleteConnectPort(PortHandle) \
		FastSystemCall(17, 0, PortHandle)

#define _NtConnectPort(PortHandle, PortName, SecurityQos, ClientView, ServerView, MaxMessageLength, ConnectInformation, ConnectInformationLength) \
		FastSystemCall(18, 0, PortHandle, PortName, SecurityQos, ClientView, ServerView, MaxMessageLength, ConnectInformation, ConnectInformationLength)

#define _NtContinue(Context, TestAlert) \
		FastSystemCall(19, 0, Context, TestAlert)

#define _NtCreateDirectoryObject(DirectoryHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(20, 0, DirectoryHandle, DesiredAccess, ObjectAttributes)

#define _NtCreateEvent(EventHandle, DesiredAccess, ObjectAttributes, EventType, InitialState) \
		FastSystemCall(21, 0, EventHandle, DesiredAccess, ObjectAttributes, EventType, InitialState)

#define _NtCreateEventPair(EventPairHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(22, 0, EventPairHandle, DesiredAccess, ObjectAttributes)

#define _NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength) \
		FastSystemCall(23, 0, FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength)

#define _NtCreateIoCompletion(IoCompletionHandle, DesiredAccess, ObjectAttributes, NumberOfConcurrentThreads) \
		FastSystemCall(24, 0, IoCompletionHandle, DesiredAccess, ObjectAttributes, NumberOfConcurrentThreads)

#define _NtCreateKey(KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition) \
		FastSystemCall(25, 0, KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition)

#define _NtCreateMailslotFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, CreateOptions, InBufferSize, MaxMessageSize, ReadTime) \
		FastSystemCall(26, 0, FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, CreateOptions, InBufferSize, MaxMessageSize, ReadTime)

#define _NtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner) \
		FastSystemCall(27, 0, MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner)

#define _NtCreateNamedPipeFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, CreateDisposition, CreateOptions, TypeMessage, ReadmodeMessage, Nonblocking, MaxInstances, InBufferSize, OutBufferSize, DefaultTime) \
		FastSystemCall(28, 0, FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, CreateDisposition, CreateOptions, TypeMessage, ReadmodeMessage, Nonblocking, MaxInstances, InBufferSize, OutBufferSize, DefaultTime)

#define _NtCreatePagingFile(FileName, InitialSize, MaximumSize, Priority) \
		FastSystemCall(29, 0, FileName, InitialSize, MaximumSize, Priority)

#define _NtCreatePort(PortHandle, ObjectAttributes, MaxConnectionInfoLength, MaxMessageLength, MaxPoolUsage) \
		FastSystemCall(30, 0, PortHandle, ObjectAttributes, MaxConnectionInfoLength, MaxMessageLength, MaxPoolUsage)

#define _NtCreateProcess(ProcessHandle, DesiredAccess, ObjectAttributes, InheritFromProcessHandle, InheritHandles, SectionHandle, DebugPort, ExceptionPort) \
		FastSystemCall(31, 0, ProcessHandle, DesiredAccess, ObjectAttributes, InheritFromProcessHandle, InheritHandles, SectionHandle, DebugPort, ExceptionPort)

#define _NtCreateProfile(ProfileHandle, ProcessHandle, Base, Size, BucketShift, Buffer, BufferLength, Source, ProcessorMask) \
		FastSystemCall(32, 0, ProfileHandle, ProcessHandle, Base, Size, BucketShift, Buffer, BufferLength, Source, ProcessorMask)

#define _NtCreateSection(SectionHandle, DesiredAccess, ObjectAttributes, SectionSize, Protect, Attributes, FileHandle) \
		FastSystemCall(33, 0, SectionHandle, DesiredAccess, ObjectAttributes, SectionSize, Protect, Attributes, FileHandle)

#define _NtCreateSemaphore(SemaphoreHandle, DesiredAccess, ObjectAttributes, InitialCount, MaximumCount) \
		FastSystemCall(34, 0, SemaphoreHandle, DesiredAccess, ObjectAttributes, InitialCount, MaximumCount)

#define _NtCreateSymbolicLinkObject(SymbolicLinkHandle, DesiredAccess, ObjectAttributes, TargetName) \
		FastSystemCall(35, 0, SymbolicLinkHandle, DesiredAccess, ObjectAttributes, TargetName)

#define _NtCreateThread(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, ClientId, ThreadContext, UserStack, CreateSuspended) \
		FastSystemCall(36, 0, ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, ClientId, ThreadContext, UserStack, CreateSuspended)

#define _NtCreateTimer(TimerHandle, DesiredAccess, ObjectAttributes, TimerType) \
		FastSystemCall(37, 0, TimerHandle, DesiredAccess, ObjectAttributes, TimerType)

#define _NtCreateToken(TokenHandle, DesiredAccess, ObjectAttributes, Type, AuthenticationId, ExpirationTime, User, Groups, Privileges, Owner, PrimaryGroup, DefaultDacl, Source) \
		FastSystemCall(38, 0, TokenHandle, DesiredAccess, ObjectAttributes, Type, AuthenticationId, ExpirationTime, User, Groups, Privileges, Owner, PrimaryGroup, DefaultDacl, Source)

#define _NtDelayExecution(Alertable, Interval) \
		FastSystemCall(39, 0, Alertable, Interval)

#define _NtDeleteAtom(Atom) \
		FastSystemCall(40, 0, Atom)

#define _NtDeleteFile(ObjectAttributes) \
		FastSystemCall(41, 0, ObjectAttributes)

#define _NtDeleteKey(KeyHandle) \
		FastSystemCall(42, 0, KeyHandle)

#define _NtDeleteObjectAuditAlarm(SubsystemName, Id, GenerateOnClose) \
		FastSystemCall(43, 0, SubsystemName, Id, GenerateOnClose)

#define _NtDeleteValueKey(KeyHandle, ValueName) \
		FastSystemCall(44, 0, KeyHandle, ValueName)

#define _NtDeviceIoControlFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, IoControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength) \
		FastSystemCall(45, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, IoControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength)

#define _NtDisplayString(String) \
		FastSystemCall(46, 0, String)

#define _NtDuplicateObject(SourceProcessHandle, SourceHandle, TargetProcessHandle, TargetHandle, DesiredAccess, Attributes, Options) \
		FastSystemCall(47, 0, SourceProcessHandle, SourceHandle, TargetProcessHandle, TargetHandle, DesiredAccess, Attributes, Options)

#define _NtDuplicateToken(ExistingTokenHandle, DesiredAccess, ObjectAttributes, EffectiveOnly, TokenType, NewTokenHandle) \
		FastSystemCall(48, 0, ExistingTokenHandle, DesiredAccess, ObjectAttributes, EffectiveOnly, TokenType, NewTokenHandle)

#define _NtEnumerateKey(KeyHandle, Index, KeyInformationClass, KeyInformation, KeyInformationLength, ResultLength) \
		FastSystemCall(49, 0, KeyHandle, Index, KeyInformationClass, KeyInformation, KeyInformationLength, ResultLength)

#define _NtEnumerateValueKey(KeyHandle, Index, KeyValueInformationClass, KeyValueInformation, KeyValueInformationLength, ResultLength) \
		FastSystemCall(50, 0, KeyHandle, Index, KeyValueInformationClass, KeyValueInformation, KeyValueInformationLength, ResultLength)

#define _NtExtendSection(SectionHandle, SectionSize) \
		FastSystemCall(51, 0, SectionHandle, SectionSize)

#define _NtFindAtom(String, StringLength, Atom) \
		FastSystemCall(52, 0, String, StringLength, Atom)

#define _NtFlushBuffersFile(FileHandle, IoStatusBlock) \
		FastSystemCall(53, 0, FileHandle, IoStatusBlock)

#define _NtFlushInstructionCache(ProcessHandle, BaseAddress, FlushSize) \
		FastSystemCall(54, 0, ProcessHandle, BaseAddress, FlushSize)

#define _NtFlushKey(KeyHandle) \
		FastSystemCall(55, 0, KeyHandle)

#define _NtFlushVirtualMemory(ProcessHandle, BaseAddress, FlushSize, IoStatusBlock) \
		FastSystemCall(56, 0, ProcessHandle, BaseAddress, FlushSize, IoStatusBlock)

#define _NtFlushWriteBuffer() \
		FastSystemCall(57, 0)

#define _NtFreeVirtualMemory(ProcessHandle, BaseAddress, FreeSize, FreeType) \
		FastSystemCall(58, 0, ProcessHandle, BaseAddress, FreeSize, FreeType)

#define _NtFsControlFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FsControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength) \
		FastSystemCall(59, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FsControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength)

#define _NtGetContextThread(ThreadHandle, Context) \
		FastSystemCall(60, 0, ThreadHandle, Context)

#define _NtGetPlugPlayEvent(Reserved1, Reserved2, Buffer, BufferLength) \
		FastSystemCall(61, 0, Reserved1, Reserved2, Buffer, BufferLength)

#define _NtImpersonateClientOfPort(PortHandle, Message) \
		FastSystemCall(62, 0, PortHandle, Message)

#define _NtImpersonateThread(ThreadHandle, TargetThreadHandle, SecurityQos) \
		FastSystemCall(63, 0, ThreadHandle, TargetThreadHandle, SecurityQos)

#define _NtInitializeRegistry(Setup) \
		FastSystemCall(64, 0, Setup)

#define _NtListenPort(PortHandle, Message) \
		FastSystemCall(65, 0, PortHandle, Message)

#define _NtLoadDriver(DriverServiceName) \
		FastSystemCall(66, 0, DriverServiceName)

#define _NtLoadKey(KeyObjectAttributes, FileObjectAttributes) \
		FastSystemCall(67, 0, KeyObjectAttributes, FileObjectAttributes)

#define _NtLoadKey2(KeyObjectAttributes, FileObjectAttributes, Flags) \
		FastSystemCall(68, 0, KeyObjectAttributes, FileObjectAttributes, Flags)

#define _NtLockFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, LockOffset, LockLength, Key, FailImmediately, ExclusiveLock) \
		FastSystemCall(69, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, LockOffset, LockLength, Key, FailImmediately, ExclusiveLock)

#define _NtLockVirtualMemory(ProcessHandle, BaseAddress, LockSize, LockType) \
		FastSystemCall(70, 0, ProcessHandle, BaseAddress, LockSize, LockType)

#define _NtMakeTemporaryObject() \
		FastSystemCall(71, 0)

#define _NtMapViewOfSection(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect) \
		FastSystemCall(72, 0, SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect)

#define _NtNotifyChangeDirectoryFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, BufferLength, NotifyFilter, WatchSubtree) \
		FastSystemCall(73, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, BufferLength, NotifyFilter, WatchSubtree)

#define _NtNotifyChangeKey(KeyHandle, EventHandle, ApcRoutine, ApcContext, IoStatusBlock, NotifyFilter, WatchSubtree, Buffer, BufferLength, Asynchronous) \
		FastSystemCall(74, 0, KeyHandle, EventHandle, ApcRoutine, ApcContext, IoStatusBlock, NotifyFilter, WatchSubtree, Buffer, BufferLength, Asynchronous)

#define _NtOpenDirectoryObject(DirectoryHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(75, 0, DirectoryHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenEvent(EventHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(76, 0, EventHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenEventPair(EventPairHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(77, 0, EventPairHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions) \
		FastSystemCall(78, 0, FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions)

#define _NtOpenIoCompletion(IoCompletionHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(79, 0, IoCompletionHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenKey(KeyHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(80, 0, KeyHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenMutant(MutantHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(81, 0, MutantHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenObjectAuditAlarm(SubsystemName, HandleId, ObjectTypeName, ObjectName, SecurityDescriptor, TokenHandle, DesiredAccess, GrantedAccess, Privileges, ObjectCreation, AccessGranted, GenerateOnClose) \
		FastSystemCall(82, 0, SubsystemName, HandleId, ObjectTypeName, ObjectName, SecurityDescriptor, TokenHandle, DesiredAccess, GrantedAccess, Privileges, ObjectCreation, AccessGranted, GenerateOnClose)

#define _NtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId) \
		FastSystemCall(83, 0, ProcessHandle, DesiredAccess, ObjectAttributes, ClientId)

#define _NtOpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle) \
		FastSystemCall(84, 0, ProcessHandle, DesiredAccess, TokenHandle)

#define _NtOpenSection(SectionHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(85, 0, SectionHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenSemaphore(SemaphoreHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(86, 0, SemaphoreHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenSymbolicLinkObject(SymbolicLinkHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(87, 0, SymbolicLinkHandle, DesiredAccess, ObjectAttributes)

#define _NtOpenThread(ThreadHandle, DesiredAccess, ObjectAttributes, ClientId) \
		FastSystemCall(88, 0, ThreadHandle, DesiredAccess, ObjectAttributes, ClientId)

#define _NtOpenThreadToken(ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle) \
		FastSystemCall(89, 0, ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle)

#define _NtOpenTimer(TimerHandle, DesiredAccess, ObjectAttributes) \
		FastSystemCall(90, 0, TimerHandle, DesiredAccess, ObjectAttributes)

#define _NtPlugPlayControl(ControlCode, Buffer, BufferLength) \
		FastSystemCall(91, 0, ControlCode, Buffer, BufferLength)

#define _NtPrivilegeCheck(TokenHandle, RequiredPrivileges, Result) \
		FastSystemCall(92, 0, TokenHandle, RequiredPrivileges, Result)

#define _NtPrivilegeObjectAuditAlarm(SubsystemName, Id, TokenHandle, DesiredAccess, Privileges, AccessGranted) \
		FastSystemCall(93, 0, SubsystemName, Id, TokenHandle, DesiredAccess, Privileges, AccessGranted)

#define _NtPrivilegedServiceAuditAlarm(SubsystemName, ServiceName, TokenHandle, Privileges, AccessGranted) \
		FastSystemCall(94, 0, SubsystemName, ServiceName, TokenHandle, Privileges, AccessGranted)

#define _NtProtectVirtualMemory(ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect) \
		FastSystemCall(95, 0, ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect)

#define _NtPulseEvent(EventHandle, PreviousState) \
		FastSystemCall(96, 0, EventHandle, PreviousState)

#define _NtQueryAttributesFile(ObjectAttributes, FileInformation) \
		FastSystemCall(97, 0, ObjectAttributes, FileInformation)

#define _NtQueryDefaultLocale(ThreadOrSystem, Locale) \
		FastSystemCall(98, 0, ThreadOrSystem, Locale)

#define _NtQueryDirectoryFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass, ReturnSingleEntry, FileName, RestartScan) \
		FastSystemCall(99, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass, ReturnSingleEntry, FileName, RestartScan)

#define _NtQueryDirectoryObject(DirectoryHandle, Buffer, BufferLength, ReturnSingleEntry, RestartScan, Context, ReturnLength) \
		FastSystemCall(100, 0, DirectoryHandle, Buffer, BufferLength, ReturnSingleEntry, RestartScan, Context, ReturnLength)

#define _NtQueryEaFile(FileHandle, IoStatusBlock, Buffer, BufferLength, ReturnSingleEntry, EaList, EaListLength, EaIndex, RestartScan) \
		FastSystemCall(101, 0, FileHandle, IoStatusBlock, Buffer, BufferLength, ReturnSingleEntry, EaList, EaListLength, EaIndex, RestartScan)

#define _NtQueryEvent(EventHandle, EventInformationClass, EventInformation, EventInformationLength, ResultLength) \
		FastSystemCall(102, 0, EventHandle, EventInformationClass, EventInformation, EventInformationLength, ResultLength)

#define _NtQueryFullAttributesFile(ObjectAttributes, FileInformation) \
		FastSystemCall(103, 0, ObjectAttributes, FileInformation)

#define _NtQueryInformationAtom(Atom, AtomInformationClass, AtomInformation, AtomInformationLength, ReturnLength) \
		FastSystemCall(104, 0, Atom, AtomInformationClass, AtomInformation, AtomInformationLength, ReturnLength)

#define _NtQueryInformationFile(FileHandle, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass) \
		FastSystemCall(105, 0, FileHandle, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass)

#define _NtQueryInformationPort(PortHandle, PortInformationClass, PortInformation, PortInformationLength, ReturnLength) \
		FastSystemCall(106, 0, PortHandle, PortInformationClass, PortInformation, PortInformationLength, ReturnLength)

#define _NtQueryInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength) \
		FastSystemCall(107, 0, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength)

#define _NtQueryInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength) \
		FastSystemCall(108, 0, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength)

#define _NtQueryInformationToken(TokenHandle, TokenInformationClass, TokenInformation, TokenInformationLength, ReturnLength) \
		FastSystemCall(109, 0, TokenHandle, TokenInformationClass, TokenInformation, TokenInformationLength, ReturnLength)

#define _NtQueryIntervalProfile(Source, Interval) \
		FastSystemCall(110, 0, Source, Interval)

#define _NtQueryIoCompletion(IoCompletionHandle, IoCompletionInformationClass, IoCompletionInformation, IoCompletionInformationLength, ResultLength) \
		FastSystemCall(111, 0, IoCompletionHandle, IoCompletionInformationClass, IoCompletionInformation, IoCompletionInformationLength, ResultLength)

#define _NtQueryKey(KeyHandle, KeyInformationClass, KeyInformation, KeyInformationLength, ResultLength) \
		FastSystemCall(112, 0, KeyHandle, KeyInformationClass, KeyInformation, KeyInformationLength, ResultLength)

#define _NtQueryMultipleValueKey(KeyHandle, ValueList, NumberOfValues, Buffer, Length, ReturnLength) \
		FastSystemCall(113, 0, KeyHandle, ValueList, NumberOfValues, Buffer, Length, ReturnLength)

#define _NtQueryMutant(MutantHandle, MutantInformationClass, MutantInformation, MutantInformationLength, ResultLength) \
		FastSystemCall(114, 0, MutantHandle, MutantInformationClass, MutantInformation, MutantInformationLength, ResultLength)

#define _NtQueryObject(ObjectHandle, ObjectInformationClass, ObjectInformation, ObjectInformationLength, ReturnLength) \
		FastSystemCall(115, 0, ObjectHandle, ObjectInformationClass, ObjectInformation, ObjectInformationLength, ReturnLength)

#define _NtQueryPerformanceCounter(PerformanceCount, PerformanceFrequency) \
		FastSystemCall(116, 0, PerformanceCount, PerformanceFrequency)

#define _NtQuerySection(SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ResultLength) \
		FastSystemCall(117, 0, SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ResultLength)

#define _NtQuerySecurityObject(Handle, SecurityInformation, SecurityDescriptor, SecurityDescriptorLength, ReturnLength) \
		FastSystemCall(118, 0, Handle, SecurityInformation, SecurityDescriptor, SecurityDescriptorLength, ReturnLength)

#define _NtQuerySemaphore(SemaphoreHandle, SemaphoreInformationClass, SemaphoreInformation, SemaphoreInformationLength, ResultLength) \
		FastSystemCall(119, 0, SemaphoreHandle, SemaphoreInformationClass, SemaphoreInformation, SemaphoreInformationLength, ResultLength)

#define _NtQuerySymbolicLinkObject(SymbolicLinkHandle, TargetName, ReturnLength) \
		FastSystemCall(120, 0, SymbolicLinkHandle, TargetName, ReturnLength)

#define _NtQuerySystemEnvironmentValue(Name, Value, ValueLength, ReturnLength) \
		FastSystemCall(121, 0, Name, Value, ValueLength, ReturnLength)

#define _NtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength) \
		FastSystemCall(122, 0, SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength)

#define _NtQuerySystemTime(CurrentTime) \
		FastSystemCall(123, 0, CurrentTime)

#define _NtQueryTimer(TimerHandle, TimerInformationClass, TimerInformation, TimerInformationLength, ResultLength) \
		FastSystemCall(124, 0, TimerHandle, TimerInformationClass, TimerInformation, TimerInformationLength, ResultLength)

#define _NtQueryTimerResolution(CoarsestResolution, FinestResolution, ActualResolution) \
		FastSystemCall(125, 0, CoarsestResolution, FinestResolution, ActualResolution);

#define _NtQueryValueKey(KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, KeyValueInformationLength, ResultLength) \
		FastSystemCall(126, 0, KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, KeyValueInformationLength, ResultLength)

#define _NtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength) \
		FastSystemCall(127, 0, ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength)

#define _NtQueryVolumeInformationFile(FileHandle, IoStatusBlock, VolumeInformation, VolumeInformationLength, VolumeInformationClass) \
		FastSystemCall(128, 0, FileHandle, IoStatusBlock, VolumeInformation, VolumeInformationLength, VolumeInformationClass)

#define _NtQueueApcThread(ThreadHandle, ApcRoutine, ApcContext, Argument1, Argument2) \
		FastSystemCall(129, 0, ThreadHandle, ApcRoutine, ApcContext, Argument1, Argument2)

#define _NtRaiseException(ExceptionRecord, Context, SearchFrames) \
		FastSystemCall(130, 0, ExceptionRecord, Context, SearchFrames)

#define _NtRaiseHardError(Status, NumberOfArguments, StringArgumentsMask, Arguments, ResponseOption, Response) \
		FastSystemCall(131, 0, Status, NumberOfArguments, StringArgumentsMask, Arguments, ResponseOption, Response)

#define _NtReadFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key) \
		FastSystemCall(132, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key)

#define _NtReadFileScatter(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key) \
		FastSystemCall(133, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key)

#define _NtReadRequestData(PortHandle, Message, Index, Buffer, BufferLength, ReturnLength) \
		FastSystemCall(134, 0, PortHandle, Message, Index, Buffer, BufferLength, ReturnLength)

#define _NtReadVirtualMemory(ProcessHandle, BaseAddress, Buffer, BufferLength, ReturnLength) \
		FastSystemCall(135, 0, ProcessHandle, BaseAddress, Buffer, BufferLength, ReturnLength)

#define _NtRegisterThreadTerminatePort(PortHandle) \
		FastSystemCall(136, 0, PortHandle)

#define _NtReleaseMutant(MutantHandle, PreviousState) \
		FastSystemCall(137, 0, MutantHandle, PreviousState)

#define _NtReleaseSemaphore(SemaphoreHandle, ReleaseCount, PPreviousCount) \
		FastSystemCall(138, 0, SemaphoreHandle, ReleaseCount, PPreviousCount)

#define _NtRemoveIoCompletion(IoCompletionHandle, CompletionKey, CompletionValue, IoStatusBlock, Time) \
		FastSystemCall(139, 0, IoCompletionHandle, CompletionKey, CompletionValue, IoStatusBlock, Time)

#define _NtReplaceKey(NewFileObjectAttributes, KeyHandle, OldFileObjectAttributes) \
		FastSystemCall(140, 0, NewFileObjectAttributes, KeyHandle, OldFileObjectAttributes)

#define _NtReplyPort(PortHandle, ReplyMessage) \
		FastSystemCall(141, 0, PortHandle, ReplyMessage);

#define _NtReplyWaitReceivePort(PortHandle, PortIdentifier, ReplyMessage, Message) \
		FastSystemCall(142, 0, PortHandle, PortIdentifier, ReplyMessage, Message)

#define _NtReplyWaitReplyPort(PortHandle, ReplyMessage) \
		FastSystemCall(143, 0, PortHandle, ReplyMessage)

#define _NtRequestPort(PortHandle, RequestMessage) \
		FastSystemCall(144, 0, PortHandle, RequestMessage)

#define _NtRequestWaitReplyPort(PortHandle, RequestMessage, ReplyMessage) \
		FastSystemCall(145, 0, PortHandle, RequestMessage, ReplyMessage)

#define _NtResetEvent(EventHandle, PreviousState) \
		FastSystemCall(146, 0, EventHandle, PreviousState)

#define _NtRestoreKey(KeyHandle, FileHandle, Flags) \
		FastSystemCall(147, 0, KeyHandle, FileHandle, Flags)

#define _NtResumeThread(ThreadHandle, PreviousSuspendCount) \
		FastSystemCall(148, 0, ThreadHandle, PreviousSuspendCount)

#define _NtSaveKey(KeyHandle, FileHandle) \
		FastSystemCall(149, 0, KeyHandle, FileHandle)

#define _NtSetContextThread(ThreadHandle, Context) \
		FastSystemCall(150, 0, ThreadHandle, Context)

#define _NtSetDefaultHardErrorPort(PortHandle) \
		FastSystemCall(151, 0, PortHandle)

#define _NtSetDefaultLocale(ThreadOrSystem, Locale) \
		FastSystemCall(152, 0, ThreadOrSystem, Locale)

#define _NtSetEaFile(FileHandle, IoStatusBlock, Buffer, BufferLength) \
		FastSystemCall(153, 0, FileHandle, IoStatusBlock, Buffer, BufferLength)

#define _NtSetEvent(EventHandle, PreviousState) \
		FastSystemCall(154, 0, EventHandle, PreviousState)

#define _NtSetHighEventPair(EventPairHandle) \
		FastSystemCall(155, 0, EventPairHandle)

#define _NtSetHighWaitLowEventPair(EventPairHandle) \
		FastSystemCall(156, 0, EventPairHandle)

#define _NtSetInformationFile(FileHandle, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass) \
		FastSystemCall(157, 0, FileHandle, IoStatusBlock, FileInformation, FileInformationLength, FileInformationClass)

#define _NtSetInformationKey(KeyHandle, KeyInformationClass, KeyInformation, KeyInformationLength) \
		FastSystemCall(158, 0, KeyHandle, KeyInformationClass, KeyInformation, KeyInformationLength)

#define _NtSetInformationObject(ObjectHandle, ObjectInformationClass, ObjectInformation, ObjectInformationLength) \
		FastSystemCall(159, 0, ObjectHandle, ObjectInformationClass, ObjectInformation, ObjectInformationLength)

#define _NtSetInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength) \
		FastSystemCall(160, 0, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength)

#define _NtSetInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength) \
		FastSystemCall(161, 0, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength)

#define _NtSetInformationToken(TokenHandle, TokenInformationClass, TokenInformation, TokenInformationLength) \
		FastSystemCall(162, 0, TokenHandle, TokenInformationClass, TokenInformation, TokenInformationLength)

#define _NtSetIntervalProfile(Interval, Source) \
		FastSystemCall(163, 0, Interval, Source)

#define _NtSetIoCompletion(IoCompletionHandle, CompletionKey, CompletionValue, Status, Information) \
		FastSystemCall(164, 0, IoCompletionHandle, CompletionKey, CompletionValue, Status, Information)

#define _NtSetLdtEntries(Selector1, LdtEntry1, Selector2, LdtEntry2) \
		FastSystemCall(165, 0, Selector1, LdtEntry1, Selector2, LdtEntry2)

#define _NtSetLowEventPair(EventPairHandle) \
		FastSystemCall(166, 0, EventPairHandle)

#define _NtSetLowWaitHighEventPair(EventPairHandle) \
		FastSystemCall(167, 0, EventPairHandle)

#define _NtSetSecurityObject(Handle, SecurityInformation, SecurityDescriptor) \
		FastSystemCall(168, 0, Handle, SecurityInformation, SecurityDescriptor)

#define _NtSetSystemEnvironmentValue(Name, Value) \
		FastSystemCall(169, 0, Name, Value)

#define _NtSetSystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength) \
		FastSystemCall(170, 0, SystemInformationClass, SystemInformation, SystemInformationLength)

#define _NtSetSystemPowerState(SystemAction, MinSystemState, Flags) \
		FastSystemCall(171, 0, SystemAction, MinSystemState, Flags)

#define _NtSetSystemTime(NewTime, OldTime) \
		FastSystemCall(172, 0,  NewTime, OldTime);

#define _NtSetTimer(TimerHandle, DueTime, TimerApcRoutine, TimerContext, Resume, Period, PreviousState) \
		FastSystemCall(173, 0, TimerHandle, DueTime, TimerApcRoutine, TimerContext, Resume, Period, PreviousState)

#define _NtSetTimerResolution(RequestedResolution, Set, ActualResolution) \
		FastSystemCall(174, 0, RequestedResolution, Set, ActualResolution)

#define _NtSetValueKey(KeyHandle, ValueName, TitleIndex, Type, Data, DataSize) \
		FastSystemCall(175, 0, KeyHandle, ValueName, TitleIndex, Type, Data, DataSize)

#define _NtSetVolumeInformationFile(FileHandle, IoStatusBlock, Buffer, BufferLength, VolumeInformationClass) \
		FastSystemCall(176, 0, FileHandle, IoStatusBlock, Buffer, BufferLength, VolumeInformationClass)

#define _NtShutdownSystem(Action) \
		FastSystemCall(177, 0, Action)

#define _NtSignalAndWaitForSingleObject(HandleToSignal, HandleToWait, Alertable, Time) \
		FastSystemCall(178, 0, HandleToSignal, HandleToWait, Alertable, Time)

#define _NtStartProfile(ProfileHandle) \
		FastSystemCall(179, 0, ProfileHandle)

#define _NtStopProfile(ProfileHandle) \
		FastSystemCall(180, 0, ProfileHandle)

#define _NtSuspendThread(ThreadHandle, PreviousSuspendCount) \
		FastSystemCall(181, 0, ThreadHandle, PreviousSuspendCount)

#define _NtSystemDebugControl(ControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnLength) \
		FastSystemCall(182, 0, ControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnLength)

#define _NtTerminateProcess(ProcessHandle, ExitStatus) \
		FastSystemCall(183, 0, ProcessHandle, ExitStatus)

#define _NtTerminateThread(ThreadHandle, ExitStatus) \
		FastSystemCall(184, 0, ThreadHandle, ExitStatus)

#define _NtTestAlert() \
		FastSystemCall(185, 0)

#define _NtUnloadDriver(DriverServiceName) \
		FastSystemCall(186, 0, DriverServiceName);

#define _NtUnloadKey(KeyObjectAttributes) \
		FastSystemCall(187, 0, KeyObjectAttributes)

#define _NtUnlockFile(FileHandle, IoStatusBlock, LockOffset, LockLength, Key) \
		FastSystemCall(188, 0, FileHandle, IoStatusBlock, LockOffset, LockLength, Key)

#define _NtUnlockVirtualMemory(ProcessHandle, BaseAddress, LockSize, LockType) \
		FastSystemCall(189, 0, ProcessHandle, BaseAddress, LockSize, LockType)

#define _NtUnmapViewOfSection(ProcessHandle, BaseAddress) \
		FastSystemCall(190, 0, ProcessHandle, BaseAddress)

#define _NtVdmControl(Service, ServiceData) \
		FastSystemCall(191, 0, Service, ServiceData)

#define _NtWaitForMultipleObjects(HandleCount, Handles, WaitType, Alertable, Time) \
		FastSystemCall(192, 0, HandleCount, Handles, WaitType, Alertable, Time)

#define _NtWaitForSingleObject(Handle, Alertable, Time) \
		FastSystemCall(193, 0, Handle, Alertable, Time)

#define _NtWaitHighEventPair(EventPairHandle) \
		FastSystemCall(194, 0, EventPairHandle)

#define _NtWaitLowEventPair(EventPairHandle) \
		FastSystemCall(195, 0, EventPairHandle)

#define _NtWriteFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key) \
		FastSystemCall(196, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key)

#define _NtWriteFileGather(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key) \
		FastSystemCall(197, 0, FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key)

#define _NtWriteRequestData(PortHandle, Message, Index, Buffer, BufferLength, ReturnLength) \
		FastSystemCall(198, 0, PortHandle, Message, Index, Buffer, BufferLength, ReturnLength)

#define _NtWriteVirtualMemory(ProcessHandle, BaseAddress, Buffer, BufferLength, ReturnLength) \
		FastSystemCall(199, 0, ProcessHandle, BaseAddress, Buffer, BufferLength, ReturnLength)

#define _NtYieldExecution() \
		FastSystemCall(200, 0)