package com.breadwallet.corecrypto;

import com.breadwallet.crypto.AddressScheme;
import com.breadwallet.crypto.CryptoApi;
import com.breadwallet.crypto.Network;
import com.breadwallet.crypto.System;
import com.breadwallet.crypto.Transfer;
import com.breadwallet.crypto.Wallet;
import com.breadwallet.crypto.WalletManager;
import com.breadwallet.crypto.WalletManagerMode;
import com.breadwallet.crypto.WalletManagerState;
import com.breadwallet.crypto.blockchaindb.BlockchainDb;
import com.breadwallet.crypto.events.network.NetworkEvent;
import com.breadwallet.crypto.events.system.SystemEvent;
import com.breadwallet.crypto.events.system.SystemListener;
import com.breadwallet.crypto.events.system.SystemNetworkAddedEvent;
import com.breadwallet.crypto.events.transfer.TranferEvent;
import com.breadwallet.crypto.events.wallet.WalletEvent;
import com.breadwallet.crypto.events.walletmanager.WalletManagerChangedEvent;
import com.breadwallet.crypto.events.walletmanager.WalletManagerCreatedEvent;
import com.breadwallet.crypto.events.walletmanager.WalletManagerEvent;
import com.google.common.io.Files;
import com.google.common.util.concurrent.Uninterruptibles;

import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Random;
import java.util.UUID;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class SbiAIT {

    // Configurable values

    private static final int SYSTEM_COUNT = 100;

    private static final boolean IS_MAINNET = false;

    private static final Date ACCOUNT_TIMESTAMP = new Date(0);

    private static final byte[] ACCOUNT_KEY = "ginger settle marine tissue robot crane night number ramp coast roast critic"
            .getBytes(StandardCharsets.UTF_8);

    private static final String BDB_AUTH_TOKEN = "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9." +
            "eyJzdWIiOiJjNzQ5NTA2ZS02MWUzLTRjM2UtYWNiNS00OTY5NTM2ZmRhMTAiLCJpYXQiOjE1N" +
            "zI1NDY1MDAuODg3LCJleHAiOjE4ODAxMzA1MDAuODg3LCJicmQ6Y3QiOiJ1c3IiLCJicmQ6Y2" +
            "xpIjoiZGViNjNlMjgtMDM0NS00OGY2LTlkMTctY2U4MGNiZDYxN2NiIn0." +
            "460_GdAWbONxqOhWL5TEbQ7uEZi3fSNrl0E_Zg7MAg570CVcgO7rSMJvAPwaQtvIx1XFK_QZjcoNULmB8EtOdg";

    // test runtime
    private static final long RUN_TIME_MS = 30 * 60 * 1000; // 30 minutes

    // period of time to wait before failing
    private static final long FAIL_DELAY_MS = 5 * 60 * 1000; // 5 minutes

    // period of time to return failure status codes
    private static final long FAIL_PERIOD_MS = 5 * 60 * 1000; // 5 minutes

    // Test

    @Before
    public void setup() {
        // log everything at the "debug" level
        Logger rootLogger = LogManager.getLogManager().getLogger("");
        Handler[] handlers = rootLogger.getHandlers();
        rootLogger.setLevel(Level.FINE);
        for (Handler h : handlers) {
            h.setLevel(Level.FINE);
        }

        // initialize the crypto provider
        try {
            CryptoApi.initialize(CryptoApiProvider.getInstance());
        } catch (IllegalStateException e) {
            // ignore, good to go
        }
    }

    @Test
    public void sbi() {
        List<SbiSystem> sbiSystems = new ArrayList<>(SYSTEM_COUNT);

        log("Starting test");

        // create systems
        log("Creating systems");
        for (int i = 0; i < SYSTEM_COUNT; i++) {
            sbiSystems.add(new SbiSystem("SYSTEM-" + i));
        }

        // start systems
        log("Starting systems");
        for (SbiSystem sbiSystem: sbiSystems) {
            sbiSystem.start();
        }

        // sleep
        log("Sleeping");
        Uninterruptibles.sleepUninterruptibly(RUN_TIME_MS, TimeUnit.MILLISECONDS);

        // stop systems
        log("Stopping systems");
        for (SbiSystem sbiSystem: sbiSystems) {
            sbiSystem.stop();
        }

        log("Finishing test");
    }

    // System

    private static class SbiSystem implements SystemListener {

        private final String name;
        private final System system;

        SbiSystem(String name) {
            this.name = name;

            File storageDir = Files.createTempDir();
            deleteRecursively(storageDir);
            storageDir.mkdirs();

            String storagePath = storageDir.getAbsolutePath();
            Account account = createAccount();
            BlockchainDb query = createBlockchainDbWithFailureStatusCode();

            ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
            system = System.create(executor, this, account, IS_MAINNET, storagePath, query);
        }

        void start() {
            system.configure(Collections.emptyList());
        }

        void stop() {
            system.disconnectAll();
        }

        @Override
        public void handleSystemEvent(System system, SystemEvent event) {
            // create wallet manager for BTC network using API mode
            if (event instanceof SystemNetworkAddedEvent) {
                SystemNetworkAddedEvent e = (SystemNetworkAddedEvent) event;
                String currentCode = e.getNetwork().getCurrency().getCode();
                if (currentCode.equals("btc") || currentCode.equals("bch")) {
                    system.createWalletManager(e.getNetwork(), WalletManagerMode.API_ONLY, AddressScheme.BTC_LEGACY,
                            Collections.emptySet());
                }
            }

            log(name, event.toString());
        }

        @Override
        public void handleManagerEvent(System system, WalletManager manager, WalletManagerEvent event) {
            // connect wallet manager created
            if (event instanceof WalletManagerCreatedEvent) {
                manager.connect(null);
            }

            // reconnect when transitioned to disconnected
            if (event instanceof WalletManagerChangedEvent) {
                WalletManagerChangedEvent e = (WalletManagerChangedEvent) event;
                if (e.getNewState().getType().equals(WalletManagerState.Type.DISCONNECTED)) {
                    manager.connect(null);
                }
            }

            log(name, event.toString());
        }

        @Override
        public void handleWalletEvent(System system, WalletManager manager, Wallet wallet, WalletEvent event) {
            log(name, event.toString());
        }

        @Override
        public void handleTransferEvent(System system, WalletManager manager, Wallet wallet, Transfer transfer, TranferEvent event) {
            log(name, event.toString());
        }

        @Override
        public void handleNetworkEvent(System system, Network network, NetworkEvent event) {
            log(name, event.toString());
        }
    }

    // BlockchainDB

    private static BlockchainDb createBlockchainDbWithFailureStatusCode() {
        long startTime = java.lang.System.currentTimeMillis();
        Random random = new SecureRandom();
        OkHttpClient client = new OkHttpClient.Builder()
                .addNetworkInterceptor(chain -> {
                    Request request = chain.request().newBuilder()
                            .header("Authorization", "Bearer " + BDB_AUTH_TOKEN)
                            .build();

                    Uninterruptibles.sleepUninterruptibly(5 + random.nextInt(35), TimeUnit.SECONDS);

                    Response.Builder responseBuilder = chain.proceed(request).newBuilder();
                    if (java.lang.System.currentTimeMillis() >= (startTime + FAIL_DELAY_MS) &&
                        java.lang.System.currentTimeMillis() <= (startTime + FAIL_DELAY_MS + FAIL_PERIOD_MS)) {
                        responseBuilder.code(525);
                    }

                    return responseBuilder.build();
                })
                .build();
        return new BlockchainDb(client);
    }

    // Accounts

    private static Account createAccount() {
        return Account.createFromPhrase(ACCOUNT_KEY, ACCOUNT_TIMESTAMP, UUID.randomUUID().toString());
    }

    // Helpers

    private static boolean deleteRecursively(File file) {
        if (file.isDirectory()) {
            File[] fileList = file.listFiles();
            if (null != fileList) {
                for (File child : fileList) {
                    if (!deleteRecursively(child)) {
                        return false;
                    }
                }
            }
        }
        return file.delete();
    }

    // Logging

    private static final Logger LOG = Logger.getLogger(SbiAIT.class.getName());

    private static void log(String prefix, String message) {
        log(String.format("%-12s -> %s", prefix, message));
    }

    private static void log(String message) {
        LOG.log(Level.INFO, message);
    }
}
