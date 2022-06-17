



单个master情况下，Journal Size会无限制地增长，通过HA让standby master执行checkpoint master state and clean up the logs that were written prior to the checkpoints.

primary master不会执行checkpoint，因为checkpoint的时候会造成master metadata 不可改变，因此不能提供服务。因此会导致primary master的journal log无限增长。

