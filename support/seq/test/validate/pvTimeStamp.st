program pvTimeStampTest

int test = 0;
assign test to "pvTimeStampTest";
monitor test;

ss one {
    state one {
        when (delay(1)) {
            printf("sec=%d\n",pvTimeStamp(test).secPastEpoch);
            test++;
            pvPut(test,SYNC);
        } state one
    }
}
