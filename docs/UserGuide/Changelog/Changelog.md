# UK Controller Plugin Changelog

# [3.12.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.11.0...3.12.0) (2021-12-21)


### Features

* **handoff:** Default departure handoff frequencies for airfield if no sid matches ([#374](https://github.com/VATSIM-UK/uk-controller-plugin/issues/374)) ([9190c99](https://github.com/VATSIM-UK/uk-controller-plugin/commit/9190c99e74d7eb7bfbe53f39d8daa5f2dd37b251))
* **missedapproach:** Allow missed approaches to be acknowledged ([#370](https://github.com/VATSIM-UK/uk-controller-plugin/issues/370)) ([d21e45b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d21e45b65192e47bf850a889d3c7f116b1cf6f86))

# [3.11.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.10.0...3.11.0) (2021-12-19)


### Features

* **srd:** Prepopulate SRD search from flightplan ([#375](https://github.com/VATSIM-UK/uk-controller-plugin/issues/375)) ([eb21611](https://github.com/VATSIM-UK/uk-controller-plugin/commit/eb216110899ea7545296d0b7444000a16d310856))

# [3.10.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.9.0...3.10.0) (2021-12-12)


### Features

* **missedapproach:** Allow integrations to trigger missed approaches ([#373](https://github.com/VATSIM-UK/uk-controller-plugin/issues/373)) ([8614508](https://github.com/VATSIM-UK/uk-controller-plugin/commit/861450803b17a355b1d357d2b4b5597cc02796dd))

# [3.9.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.8.1...3.9.0) (2021-11-28)


### Bug Fixes

* **srd:** Cater for longer routes in SRD dialog ([#364](https://github.com/VATSIM-UK/uk-controller-plugin/issues/364)) ([32136de](https://github.com/VATSIM-UK/uk-controller-plugin/commit/32136de150d31f76bc5a2d492d1b89e51bb3d151)), closes [#358](https://github.com/VATSIM-UK/uk-controller-plugin/issues/358)


### Features

* **srd:** allow srd searches by flight level ([#360](https://github.com/VATSIM-UK/uk-controller-plugin/issues/360)) ([e7b1899](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e7b1899d57d1f4dcb48e82270ad79a21d728e44b)), closes [#358](https://github.com/VATSIM-UK/uk-controller-plugin/issues/358)
* **srd:** Indicate whether a route contains free route airspace ([#363](https://github.com/VATSIM-UK/uk-controller-plugin/issues/363)) ([ab0b62a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ab0b62a63b0e877ff712c690e1d741bc80b0ce34)), closes [#358](https://github.com/VATSIM-UK/uk-controller-plugin/issues/358)

## [3.8.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.8.0...3.8.1) (2021-11-14)


### Bug Fixes

* **notifications:** notifications not being read when only one available ([#355](https://github.com/VATSIM-UK/uk-controller-plugin/issues/355)) ([4193501](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4193501e1a5be0d3b29e365fa3cc7919494eb72d))

# [3.8.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.7.0...3.8.0) (2021-11-07)


### Features

* **missed:** Electronic missed approach coordination ([#346](https://github.com/VATSIM-UK/uk-controller-plugin/issues/346)) ([844bdf4](https://github.com/VATSIM-UK/uk-controller-plugin/commit/844bdf4dc074c75b9555ccc64d72ae48b2a629c3)), closes [#344](https://github.com/VATSIM-UK/uk-controller-plugin/issues/344) [#345](https://github.com/VATSIM-UK/uk-controller-plugin/issues/345) [#347](https://github.com/VATSIM-UK/uk-controller-plugin/issues/347) [#350](https://github.com/VATSIM-UK/uk-controller-plugin/issues/350) [#352](https://github.com/VATSIM-UK/uk-controller-plugin/issues/352) [#350](https://github.com/VATSIM-UK/uk-controller-plugin/issues/350)

# [3.7.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.6.3...3.7.0) (2021-10-26)


### Features

* **selcal:** add tag item to show selcal from flightplan remarks ([#353](https://github.com/VATSIM-UK/uk-controller-plugin/issues/353)) ([c26dc46](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c26dc46eca5c9f8ab71322cd5a202481b7480e76))

## [3.6.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.6.2...3.6.3) (2021-10-18)


### Bug Fixes

* **release+prenotes:** Broken colours in lists ([#354](https://github.com/VATSIM-UK/uk-controller-plugin/issues/354)) ([292a613](https://github.com/VATSIM-UK/uk-controller-plugin/commit/292a6134191006e602ae2d5f5e7c0960b66bae7e))

## [3.6.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.6.1...3.6.2) (2021-10-02)


### Bug Fixes

* **a11y:** Make release and prenote list header colours accessible ([e89dd19](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e89dd1929d4d277662af71549e3a52879a69318d))
* **integration:** Fix messages not being received from clients ([#343](https://github.com/VATSIM-UK/uk-controller-plugin/issues/343)) ([7d85c9b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/7d85c9b5401dcd28e29b999cb4de904cf4d28b99))

## [3.6.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.6.0...3.6.1) (2021-09-18)


### Bug Fixes

* **docs:** Fix docs to trigger CI ([04bad57](https://github.com/VATSIM-UK/uk-controller-plugin/commit/04bad57a06ab023f42f29ec089dc1c02159ae7e4))

# [3.6.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.5.0...3.6.0) (2021-09-18)


### Features

* **prenotes:** Electronic prenote messages ([#330](https://github.com/VATSIM-UK/uk-controller-plugin/issues/330)) ([8ed705d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8ed705ddfaa783feb7828c2e1781ad0ac610be83)), closes [#327](https://github.com/VATSIM-UK/uk-controller-plugin/issues/327) [#328](https://github.com/VATSIM-UK/uk-controller-plugin/issues/328) [#329](https://github.com/VATSIM-UK/uk-controller-plugin/issues/329) [#332](https://github.com/VATSIM-UK/uk-controller-plugin/issues/332) [#336](https://github.com/VATSIM-UK/uk-controller-plugin/issues/336) [#335](https://github.com/VATSIM-UK/uk-controller-plugin/issues/335) [#337](https://github.com/VATSIM-UK/uk-controller-plugin/issues/337)

# [3.5.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.4.2...3.5.0) (2021-08-22)


### Bug Fixes

* **notifications:** Notifications window not showing ([#326](https://github.com/VATSIM-UK/uk-controller-plugin/issues/326)) ([f28a677](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f28a6772858262aa3d2a26e2524dc9591a470e61))


### Features

* **stands:** Allow external programs to view and modify stand assignments ([#320](https://github.com/VATSIM-UK/uk-controller-plugin/issues/320)) ([b0bc292](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b0bc292ac8853c033f7c344c3500415ef584459b))

## [3.4.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.4.1...3.4.2) (2021-08-08)


### Bug Fixes

* **integration:** Integrations not staying connected ([#312](https://github.com/VATSIM-UK/uk-controller-plugin/issues/312)) ([0900898](https://github.com/VATSIM-UK/uk-controller-plugin/commit/09008989f3c3e0ce6698131098ab173d9e95ed62))

## [3.4.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.4.0...3.4.1) (2021-08-06)


### Bug Fixes

* **bootstrap:** fix euroscope crash in development mode ([#318](https://github.com/VATSIM-UK/uk-controller-plugin/issues/318)) ([d66039d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d66039dc09e261256dd9159f0292c9f4276b4d5f)), closes [#317](https://github.com/VATSIM-UK/uk-controller-plugin/issues/317)
* **edt:** fix timezone issue with estimated departure times ([#316](https://github.com/VATSIM-UK/uk-controller-plugin/issues/316)) ([09396da](https://github.com/VATSIM-UK/uk-controller-plugin/commit/09396dad1de7f0b39a9fc93a1b7b49a10eafb59f)), closes [#190](https://github.com/VATSIM-UK/uk-controller-plugin/issues/190)
* **push-events:** fix potential crash situation in push events ([#315](https://github.com/VATSIM-UK/uk-controller-plugin/issues/315)) ([ff617be](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ff617bef3c4af9edbb10a59ab9d02353519fd5bb)), closes [#313](https://github.com/VATSIM-UK/uk-controller-plugin/issues/313)
* **stands:** fix concurrency issues with stand assignments ([#311](https://github.com/VATSIM-UK/uk-controller-plugin/issues/311)) ([35b9e2a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/35b9e2aee816ad18eac941832bbc377d742ec949)), closes [#310](https://github.com/VATSIM-UK/uk-controller-plugin/issues/310)

# [3.4.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.3.1...3.4.0) (2021-07-30)


### Features

* **history-trails:** Add dot filling, line-style dots, and track-rotation ([#306](https://github.com/VATSIM-UK/uk-controller-plugin/issues/306)) ([07a2e5e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/07a2e5e7d34519062e3aa9e921f5c4227e3a4481))

## [3.3.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.3.0...3.3.1) (2021-07-29)


### Bug Fixes

* **plugin-events:** fix race condition when requesting event updates ([#307](https://github.com/VATSIM-UK/uk-controller-plugin/issues/307)) ([5da1b7c](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5da1b7c444217d8aa2bb0dae6a4185e28319ee32))

# [3.3.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.2.0...3.3.0) (2021-07-26)


### Features

* **integration:** Introduce integration and event sharing with third-party plugins ([#303](https://github.com/VATSIM-UK/uk-controller-plugin/issues/303)) ([739930e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/739930e53b12210f541c12dbd0f55b052cfee82b)), closes [#305](https://github.com/VATSIM-UK/uk-controller-plugin/issues/305)

# [3.2.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.1.4...3.2.0) (2021-07-13)


### Features

* **departure-release:** Play sound on new request, accept and reject ([#301](https://github.com/VATSIM-UK/uk-controller-plugin/issues/301)) ([dd594c2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/dd594c2c4415a4b9cbacf01ec7709f2be1071502)), closes [#300](https://github.com/VATSIM-UK/uk-controller-plugin/issues/300)

## [3.1.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.1.3...3.1.4) (2021-07-05)


### Bug Fixes

* **departure-releases:** add acknowledgement colour for departure release status indicator ([#296](https://github.com/VATSIM-UK/uk-controller-plugin/issues/296)) ([1afcc53](https://github.com/VATSIM-UK/uk-controller-plugin/commit/1afcc5356b0ade6af422d5a3b7149cde7706473c)), closes [#295](https://github.com/VATSIM-UK/uk-controller-plugin/issues/295)
* **departure-releases:** treat releases at current minute as released at exactly now ([#298](https://github.com/VATSIM-UK/uk-controller-plugin/issues/298)) ([3e1dd6e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3e1dd6e9b6fa1976514811200d2ea3a0c5dc10f3)), closes [#297](https://github.com/VATSIM-UK/uk-controller-plugin/issues/297)

## [3.1.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.1.2...3.1.3) (2021-07-03)


### Bug Fixes

* **departure-releases:** fix colours in the release status view and add documentation ([#293](https://github.com/VATSIM-UK/uk-controller-plugin/issues/293)) ([f6ba6a1](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f6ba6a10202938ca9fc974df7df877660c3ffa9c)), closes [#290](https://github.com/VATSIM-UK/uk-controller-plugin/issues/290)
* **departure-releases:** fix crash when acknowledging departure releases ([#292](https://github.com/VATSIM-UK/uk-controller-plugin/issues/292)) ([41345f2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/41345f257b1fa5922aa9a0d349fdbae9c5de98df)), closes [#289](https://github.com/VATSIM-UK/uk-controller-plugin/issues/289)

## [3.1.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.1.1...3.1.2) (2021-07-03)


### Bug Fixes

* **departure-releases:** Fix releases not being properly approved through decision list ([#290](https://github.com/VATSIM-UK/uk-controller-plugin/issues/290)) ([b8fbdb5](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b8fbdb5776d40bb3e235a3e5430a5fbbb396165c))

## [3.1.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.1.0...3.1.1) (2021-07-03)


### Bug Fixes

* **squawks:** assign squawks with C-Mode correlation when aircraft connect ([#287](https://github.com/VATSIM-UK/uk-controller-plugin/issues/287)) ([464de37](https://github.com/VATSIM-UK/uk-controller-plugin/commit/464de37f4cecea93c8e0fda9eec3f87985fe7c86)), closes [#284](https://github.com/VATSIM-UK/uk-controller-plugin/issues/284) [#285](https://github.com/VATSIM-UK/uk-controller-plugin/issues/285) [#286](https://github.com/VATSIM-UK/uk-controller-plugin/issues/286)

# [3.1.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.0.1...3.1.0) (2021-07-02)


### Bug Fixes

* **semantic-release:** add missing semantic release plugins ([d9f7851](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d9f785110240a0e402055359039fb5570fa802f1))


### Features

* **releases:** Add electronic departure releases ([#272](https://github.com/VATSIM-UK/uk-controller-plugin/issues/272)) ([9754ad5](https://github.com/VATSIM-UK/uk-controller-plugin/commit/9754ad540e39de0164bd1de13a7628287e16be56)), closes [#267](https://github.com/VATSIM-UK/uk-controller-plugin/issues/267) [#268](https://github.com/VATSIM-UK/uk-controller-plugin/issues/268) [#269](https://github.com/VATSIM-UK/uk-controller-plugin/issues/269) [#271](https://github.com/VATSIM-UK/uk-controller-plugin/issues/271) [#281](https://github.com/VATSIM-UK/uk-controller-plugin/issues/281)

## Version 3.0.1

- Fix a bug where initial altitudes updates are repeatedly triggered on plugins running on a proxy EuroScope connection.

## Version 3.0.0

- The UK Controller Plugin is now self-updating.

## Versions before 3.0.0

For changes in versions prior to 3.0.0, please see the download section of the VATSIM UK Website.
