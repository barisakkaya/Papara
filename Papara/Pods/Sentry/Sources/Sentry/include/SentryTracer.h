#import "SentrySpanProtocol.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class SentryHub, SentryTransactionContext, SentryTraceHeader, SentryTraceContext,
    SentryDispatchQueueWrapper;

static NSTimeInterval const SentryTracerDefaultTimeout = 3.0;

@interface SentryTracer : NSObject <SentrySpan>

/**
 *Span name.
 */
@property (nonatomic, copy) NSString *name;

/**
 * The context information of the span.
 */
@property (nonatomic, readonly) SentrySpanContext *context;

/**
 * The timestamp of which the span ended.
 */
@property (nullable, nonatomic, strong) NSDate *timestamp;

/**
 * The start time of the span.
 */
@property (nullable, nonatomic, strong) NSDate *startTimestamp;

/**
 * Whether the span is finished.
 */
@property (readonly) BOOL isFinished;

@property (nullable, nonatomic, copy) void (^finishCallback)(SentryTracer *);

/**
 * Indicates whether this tracer will be finished only if all children have been finished.
 * If this property is YES and the finish function is called before all children are finished
 * the tracer will automatically finish when the last child finishes.
 */
@property (readonly) BOOL waitForChildren;

/**
 * Retrieves a trace context from this tracer.
 */
@property (nonatomic, readonly) SentryTraceContext *traceContext;

/*
 The root span of this tracer.
 */
@property (nonatomic, readonly) id<SentrySpan> rootSpan;

/*
 All the spans that where created with this tracer but rootSpan.
 */
@property (nonatomic, readonly) NSArray<id<SentrySpan>> *children;

/**
 * Init a SentryTracer with given transaction context and hub and set other fields by default
 *
 * @param transactionContext Transaction context
 * @param hub A hub to bind this transaction
 *
 * @return SentryTracer
 */
- (instancetype)initWithTransactionContext:(SentryTransactionContext *)transactionContext
                                       hub:(nullable SentryHub *)hub;

/**
 * Init a SentryTracer with given transaction context, hub and whether the tracer should wait
 * for all children to finish before it finishes.
 *
 * @param transactionContext Transaction context
 * @param hub A hub to bind this transaction
 * @param waitForChildren Whether this tracer should wait all children to finish.
 *
 * @return SentryTracer
 */
- (instancetype)initWithTransactionContext:(SentryTransactionContext *)transactionContext
                                       hub:(nullable SentryHub *)hub
                           waitForChildren:(BOOL)waitForChildren;

/**
 * Init a SentryTracer with given transaction context, hub and whether the tracer should wait
 * for all children to finish before it finishes.
 *
 * @param transactionContext Transaction context
 * @param hub A hub to bind this transaction
 * @param idleTimeout The idle time to wait until to finish the transaction.
 *
 * @return SentryTracer
 */
- (instancetype)initWithTransactionContext:(SentryTransactionContext *)transactionContext
                                       hub:(nullable SentryHub *)hub
                               idleTimeout:(NSTimeInterval)idleTimeout
                      dispatchQueueWrapper:(SentryDispatchQueueWrapper *)dispatchQueueWrapper;

- (id<SentrySpan>)startChildWithParentId:(SentrySpanId *)parentId
                               operation:(NSString *)operation
                             description:(nullable NSString *)description
    NS_SWIFT_NAME(startChild(parentId:operation:description:));

/**
 * A method to inform the tracer that a span finished.
 */
- (void)spanFinished:(id<SentrySpan>)finishedSpan;

/**
 * Get the tracer from a span.
 */
+ (nullable SentryTracer *)getTracer:(id<SentrySpan>)span;

- (void)dispatchIdleTimeout;

@end

NS_ASSUME_NONNULL_END
