# UK Controller Plugin Changelog

## [5.18.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.18.2...5.18.3) (2026-02-03)


### Bug Fixes

* Fix pack hash comparison with whitespaces ([5400bad](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5400bad1e57e1f93492843b312e36e0d0c905a4f))

## [5.18.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.18.1...5.18.2) (2026-02-02)


### Bug Fixes

* close button to close wake calculator ([654aa56](https://github.com/VATSIM-UK/uk-controller-plugin/commit/654aa56973320852e438439d3677041f8af6b3d8))

## [5.18.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.18.0...5.18.1) (2025-12-01)


### Bug Fixes

* configure debug options compile targets ([b21d96b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b21d96b8099bb7f054c9d164d52bffb1d1fdf87f))

# [5.18.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.17.4...5.18.0) (2025-11-03)


### Features

* make default intention codes blank ([80dc0e2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/80dc0e201009eb63e06cf9b4b240d62a225a65d8))

## [5.17.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.17.3...5.17.4) (2025-10-25)


### Bug Fixes

* trigger release ([83c1c2b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/83c1c2bade28602f142c0211561e8510424f40f6))

## [5.17.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.17.1...5.17.2) (2025-10-19)


### Bug Fixes

* library name typo in CMakeLists ([8aa33b5](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8aa33b5e45ed15db984175a3e1e0c45d817b6849))
* specify const on array size constant ([6e62c1b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6e62c1b46eae3be09460c8520506038a491baa01))
* use posix paths in resource script ([125b704](https://github.com/VATSIM-UK/uk-controller-plugin/commit/125b7044855ec9547d3cd0ec4c6a2e1d2993c5d7))

## [5.17.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.17.0...5.17.1) (2024-04-30)


### Bug Fixes

* change user create api key path ([1cfacd2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/1cfacd21636239614d39621103899032129d950d))

# [5.17.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.16.0...5.17.0) (2024-04-15)


### Bug Fixes

* log plugin log id ([b199bd9](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b199bd99b43ead23d4de819451da27d0c6ef79a2))
* remove testing code ([9576373](https://github.com/VATSIM-UK/uk-controller-plugin/commit/957637362bcc09f0e9993be50a11f47af7c34f12))


### Features

* log fatal exceptions to the api ([cd500c0](https://github.com/VATSIM-UK/uk-controller-plugin/commit/cd500c03676b2db7b8bbe4c134e5df2ba12f630e))

# [5.16.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.15.0...5.16.0) (2024-02-25)


### Features

* make wake calculator collapsible ([88869ac](https://github.com/VATSIM-UK/uk-controller-plugin/commit/88869ac1e211741b5b2fd2b9345ea779619f9d01))

# [5.15.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.14.0...5.15.0) (2023-12-28)


### Bug Fixes

* change colour for strong deviation ([c41e96a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c41e96a11906331c686b9746bcaa92ea5f1e5b95))


### Features

* glideslope drift tag item ([3b5f774](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3b5f774b3524d29c93c647c2a04817b03b8b765c))

# [5.14.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.5...5.14.0) (2023-12-20)


### Bug Fixes

* curl requests not sending bodies ([57dd53c](https://github.com/VATSIM-UK/uk-controller-plugin/commit/57dd53ceeb6c552b84a1ec778bdda2cc88f232a0))


### Features

* api stand auto assignments ([7c1efbf](https://github.com/VATSIM-UK/uk-controller-plugin/commit/7c1efbf604566c57728f9c02eb2b445c17b2ecbe))

## [5.13.5](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.4...5.13.5) (2023-12-10)


### Bug Fixes

* log fatal exceptions ([ddf77a2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ddf77a2d42f5251af04ae2b18501e12458ed6ec8))
* remove red herring log entry from integrations ([037adc6](https://github.com/VATSIM-UK/uk-controller-plugin/commit/037adc6a95c7be452b1277703fb8a79264cc53eb))

## [5.13.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.3...5.13.4) (2023-11-12)


### Bug Fixes

* ecfmp throws exception when flow measure has no aircraft ([fc6b238](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fc6b23806ac1db2b2ea308061fb109518349ae69)), closes [#547](https://github.com/VATSIM-UK/uk-controller-plugin/issues/547) [#538](https://github.com/VATSIM-UK/uk-controller-plugin/issues/538)

## [5.13.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.2...5.13.3) (2023-10-20)


### Bug Fixes

* initial altitudes not assigning in low pressure conditions ([3ecc4ee](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3ecc4ee61d594d45e17cbc05c00ea1ddc58c5bbb)), closes [#544](https://github.com/VATSIM-UK/uk-controller-plugin/issues/544)
* initial headings not assigning in low pressure conditions ([56fa3a2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/56fa3a2da4de105e116143eb527eec12dd572a29)), closes [#545](https://github.com/VATSIM-UK/uk-controller-plugin/issues/545)

## [5.13.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.1...5.13.2) (2023-10-14)


### Bug Fixes

* ecfmp lags ([cf4d5b3](https://github.com/VATSIM-UK/uk-controller-plugin/commit/cf4d5b3afabc9c8fc55d6301e73d8aa63266de52)), closes [#542](https://github.com/VATSIM-UK/uk-controller-plugin/issues/542)

## [5.13.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.13.0...5.13.1) (2023-09-21)


### Bug Fixes

* flow measures not showing as active ([12cdebd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/12cdebd8958810d080661435ca9a82b9afe91577))

# [5.13.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.12.4...5.13.0) (2023-09-16)


### Bug Fixes

* register flow measures for flightplan events ([7700c43](https://github.com/VATSIM-UK/uk-controller-plugin/commit/7700c43c0f04e5d215a5f40239a8fac0069c15e7))


### Features

* consider flow measures for the global fir ([d826c14](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d826c14f48df40b13c040c04d678c30521fc5385))
* ECFMP relevant flow measure tag item ([a46ea25](https://github.com/VATSIM-UK/uk-controller-plugin/commit/a46ea25b2747e94eb8dd4d8d6df577ab6f0b8637))
* limit flow measures to those relevant to controller ([0c5e84b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/0c5e84bf6f5746bc9d25f0e5b58289d61dc7d5cb))
* listing ecfmp flow measures ([a5d04ac](https://github.com/VATSIM-UK/uk-controller-plugin/commit/a5d04acd08d746afa2d3bbddf987e2703dbf1fce))
* mapping flightplans to flow measures ([90b0e73](https://github.com/VATSIM-UK/uk-controller-plugin/commit/90b0e731b70246cfd97eb1e7bfbacc8fe5a9a2cb))

## [5.12.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.12.3...5.12.4) (2023-09-15)


### Bug Fixes

* copy constructor ([eb12c25](https://github.com/VATSIM-UK/uk-controller-plugin/commit/eb12c258b93f9b4aa9e4c144aab411ac43583ed1))
* explicit copy constructors for tag functions ([0c32d66](https://github.com/VATSIM-UK/uk-controller-plugin/commit/0c32d663370991bfcc3e6fd1b718cb0d85791540))

## [5.12.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.12.2...5.12.3) (2023-07-10)


### Bug Fixes

* anti aliased history trail dots ([cd66452](https://github.com/VATSIM-UK/uk-controller-plugin/commit/cd66452b2fd70a3a7de0413d82a016994497b1cb))
* history trail filled dots ([e5d66fd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e5d66fd45c643d308326d1c80578dda2664dcd6b))

## [5.12.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.12.1...5.12.2) (2023-07-09)


### Bug Fixes

* history trail dots disappearing ([ca4a892](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ca4a892841d19252838842728da07fb26da902b3))

## [5.12.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.12.0...5.12.1) (2023-07-09)


### Performance Improvements

* iteration performance upgrades ([c9176cd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c9176cda207c202cf6c2c8a3bd3ea8c1e9d72e86))
* predetermine history trail draw function ([0d87dc5](https://github.com/VATSIM-UK/uk-controller-plugin/commit/0d87dc5d6af056bb6b70631f852ec3f9f9f78be1))
* remove timing code ([37ef4dd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/37ef4dd773aa5c948112267ea7e9203ede3c5d11))

# [5.12.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.11.0...5.12.0) (2023-06-11)


### Features

* initialiser for handoffs for integrations ([15c909b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/15c909b096979b71a116db502af58fe9f544c889))

# [5.11.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.10.0...5.11.0) (2023-06-06)


### Features

* add manual squawk option ([8158dbc](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8158dbcfd07f9cee1b3f6bdb8645bd116e688c25))
* allow users to set preference on release messages ([f8e43cf](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f8e43cf120dc1efad62e47292a589cf40eda3bd6))
* send chat area message for release requested ([5d790d7](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5d790d732ef29e3ac44a5a2294e2a2f53b12a262))
* squawk assignment menu ([43d02a2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/43d02a25e45bfc89f133c4393b98b4994cea3675))

# [5.10.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.9.1...5.10.0) (2023-05-26)


### Features

* send initialisation data to integrations ([3227a36](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3227a368c1d2bf909dcb62c83866934d1ffc9f3b))
* send intention code updates to vStrips if it loads after the plugin ([5f890dd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5f890dd22a4e5103635bd2af27dbafffc2ac42be))

# [5.10.0-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.9.1...5.10.0-beta.1) (2023-05-26)


### Features

* send initialisation data to integrations ([3227a36](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3227a368c1d2bf909dcb62c83866934d1ffc9f3b))
* send intention code updates to vStrips if it loads after the plugin ([5f890dd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5f890dd22a4e5103635bd2af27dbafffc2ac42be))

## [5.9.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.9.0...5.9.1) (2023-05-14)


### Bug Fixes

* register eventbus in loader and updater ([a23384e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/a23384e42a3c69540c0adeaa3ad5b3c1d19b3597))

# [5.9.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.8.0...5.9.0) (2023-05-14)


### Features

* easy credential setup ([2d5da9a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/2d5da9a428952029e0a9c62c7b24320bea9afd7e))

# [5.8.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.7.3...5.8.0) (2023-04-19)


### Features

* clear initial altitudes automatically on departure ([86dae0c](https://github.com/VATSIM-UK/uk-controller-plugin/commit/86dae0cd6314aed61d118ba7607caf1a6e1d10bb))
* clear initial heading automatically on departure ([de32d91](https://github.com/VATSIM-UK/uk-controller-plugin/commit/de32d9119abaaa6699a93dcd35daac6d6b339a48))

# [5.8.0-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.7.1...5.8.0-beta.1) (2023-04-07)


### Features

* send initialisation data to integrations ([3227a36](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3227a368c1d2bf909dcb62c83866934d1ffc9f3b))
* send intention code updates to vStrips if it loads after the plugin ([5f890dd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5f890dd22a4e5103635bd2af27dbafffc2ac42be))
    

## [5.7.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.7.2...5.7.3) (2023-04-16)


### Bug Fixes

* api settings selection for ES3.2.21r34 ([22c8244](https://github.com/VATSIM-UK/uk-controller-plugin/commit/22c82447d2a412b623175891a45666f4a393000e))

## [5.7.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.7.1...5.7.2) (2023-04-06)


### Bug Fixes

* api key selection window not showing ([67d690a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/67d690a82359d38f1b484f94f17c999932cee21b))
>>>>>>> upstream/main

## [5.7.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.7.0...5.7.1) (2023-03-31)


### Bug Fixes

* oceanic tag items crash ([ed8ffed](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ed8ffedf5b29af2ce265d2146749fa909054a334)), closes [#511](https://github.com/VATSIM-UK/uk-controller-plugin/issues/511)

# [5.7.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.6.1...5.7.0) (2023-02-11)


### Features

* display hold description in information tab ([43560cd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/43560cd7cae92c7ca40829497dbed3e8fabff911))
* display information for multiple published holds ([c60b62c](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c60b62c6104731f4e933f22c56a715b74fef7f4a))
* display message when no published holds found ([10904f9](https://github.com/VATSIM-UK/uk-controller-plugin/commit/10904f9d2128328c8331ed61e690c93e721f5b33))
* display outbound leg information for holds ([ac3b359](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ac3b3595c7b32529c8704c93802f9f7d24112c92))

## [5.6.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.6.0...5.6.1) (2023-01-04)


### Bug Fixes

* invalidate intention code caches on controller and fp events ([4174460](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4174460f562d21d219112a0d7c0a86d2093212b6))

# [5.6.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.5.3...5.6.0) (2023-01-03)


### Bug Fixes

* determine fir exit properly ([dad6caf](https://github.com/VATSIM-UK/uk-controller-plugin/commit/dad6cafc31fde630ac7887399adecfe30cf41732))
* send blank exit point if no point for intention code ([8a949c5](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8a949c521bbb47c97448538729aa23527a04e0aa))


### Features

* drive intention codes from api data ([690c37f](https://github.com/VATSIM-UK/uk-controller-plugin/commit/690c37ffa7ff7189436a0cef92938499812d1005))
* intention code code generators ([8b9a2f4](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8b9a2f45cee09ed179e5979faa186388489e8119))
* intention code conditions ([8740c0b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/8740c0b95f140076547f0dbb52aee640e8ee1914))
* load fir exit points from dependency ([6c46b9b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6c46b9b7acc0e617f0367ead3110c088e3bfde3e))
* load intention codes from API ([9dffdf0](https://github.com/VATSIM-UK/uk-controller-plugin/commit/9dffdf0a763aec48d304cc044c074e9ebfc46af6))
* populate SRD search using exit points from API ([160f99b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/160f99b6b06aa33d8b7a4fcddf19c47cbe154f59))
* rename intention code tag item ([fd62564](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fd62564904c4aa638c8aa18ffc193892cb281711))


### Performance Improvements

* fix more ([12257a1](https://github.com/VATSIM-UK/uk-controller-plugin/commit/12257a1097790ee664a4c1da1e4bb60ae424be6c))
* fix my silly syntax errors ([37cfc2e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/37cfc2eac4d6ed5d76a651029ab85bc3ab965e3f))
* improve gmock compile performance ([0f7621f](https://github.com/VATSIM-UK/uk-controller-plugin/commit/0f7621f6ca1b4da9918a729c4dc3bf623f2060a6))

## [5.5.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.5.2...5.5.3) (2022-12-24)


### Bug Fixes

* irish G intention code directions ([cd0534a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/cd0534a114b78024cdd2ad8096c997a644041e4b))

## [5.5.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.5.1...5.5.2) (2022-12-10)


### Bug Fixes

* TOPPA and GODOS intention code directions ([5401699](https://github.com/VATSIM-UK/uk-controller-plugin/commit/54016992031085e07965b00960cbbbc71a6385f2))

## [5.5.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.5.0...5.5.1) (2022-12-02)


### Bug Fixes

* f intention code updates ([fd0c9f1](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fd0c9f1cb5c6fbc4fc10865e26ecfe00961251ee))

# [5.5.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.6...5.5.0) (2022-10-14)


### Bug Fixes

* prenote message event handlers registering in release mode ([4e3598a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4e3598a6f09f65abb6407ef07749b8f0b5237b06))
* properly trigger events after push events ([25382df](https://github.com/VATSIM-UK/uk-controller-plugin/commit/25382df3526a9677728be5d0e21b7c3840019e1b))
* unused variable ([1582ee2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/1582ee2e50d7b1a31e949a15fe975a641d6419fa))


### Features

* add OP item to open data folder ([84a49ef](https://github.com/VATSIM-UK/uk-controller-plugin/commit/84a49eff940e4b1ea62301d821efe65ec0227bbc))
* add OP item to open logs folder ([fc4036b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fc4036b7d0346fd20d0d6eff465e49ba6499e378))
* general settings checkbox for prenote chat area messages ([22b74b7](https://github.com/VATSIM-UK/uk-controller-plugin/commit/22b74b75649c5f0da6bc7276f65ca4a850352cba))
* send a chat area message when a preonte message is cancelled ([ebf2ff7](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ebf2ff7e3549f2e056cae5b769abb5938ab8e95e))
* send message to chat area when prenote received ([af97223](https://github.com/VATSIM-UK/uk-controller-plugin/commit/af97223dca869b8d1b62bec7ef93fada11868c1d))
* send notifications to sending controller on prenote timeout ([b73eded](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b73eded2af7b7ea190ee190d546cab5ad76f2450))
* send prenote acknowledgement message to chat area ([4b021f8](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4b021f8e0f4db9a3f9286e8292a4cdfeeb2fe628))
* users can select whether they want prenote messages in chat area ([981e8af](https://github.com/VATSIM-UK/uk-controller-plugin/commit/981e8af204d8370c1678e37577e4f42be8de9f9b))

## [5.4.6](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.5...5.4.6) (2022-09-19)


### Bug Fixes

* allow handoffs with no controllers ([3ef062a](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3ef062a0c2c02d49f13794360eb4d808a5161ce7))

## [5.4.5](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.4...5.4.5) (2022-09-03)


### Bug Fixes

* treat additional squawks as invalid ([6f2a202](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6f2a202c3ca003e321021c8aa496ff4d9c6002dc))

## [5.4.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.3...5.4.4) (2022-08-20)


### Bug Fixes

* display redistributables warning on load error ([a37d810](https://github.com/VATSIM-UK/uk-controller-plugin/commit/a37d8106b1ac3bc95aa9b1eb595ffd01b1232f94))
* **handoff:** departure frequencies not updating after runway dialog change ([ad9b87b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ad9b87b0d01723604a3d68491fb9ab339f848cac)), closes [#480](https://github.com/VATSIM-UK/uk-controller-plugin/issues/480)
* LoadPlugin error message ([5b74322](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5b74322cb19e716cc9052712e0d37c142ccdbd49))

## [5.4.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.2...5.4.3) (2022-07-31)


### Bug Fixes

* remove key from prenote dependency ([b6d2c70](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b6d2c701ef713c973661483db467b646431c7887))

## [5.4.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.1...5.4.2) (2022-07-31)


### Bug Fixes

* remove key from handoffs ([e0e28a0](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e0e28a0dcec3600d4022206ebe9ed7c86cc712a6))

## [5.4.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.4.0...5.4.1) (2022-07-24)


### Bug Fixes

* **intention:** k3 intention code via larga ([f7150e4](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f7150e401d82561dce9421b845ac7128b4087180))
* **intention:** konan d1 code ([1a864a3](https://github.com/VATSIM-UK/uk-controller-plugin/commit/1a864a37e8b4ba44770c6c351420603cbd36f4a7))

# [5.4.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.3.0...5.4.0) (2022-07-11)


### Features

* **api:** use api prefix for all routes ([e9f7338](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e9f7338975717f8e7e791b81ee97e87f92ae2ea5))

# [5.3.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.2.3...5.3.0) (2022-05-19)


### Features

* **intention:** E2 Max Level Change ([50be2d4](https://github.com/VATSIM-UK/uk-controller-plugin/commit/50be2d459cba0ddde7514e1cc0687bbea66d024b))
* **intention:** Hern intention codes ([966a445](https://github.com/VATSIM-UK/uk-controller-plugin/commit/966a4457fd73e85dc3beee7cddeeb62c2ee21466))

## [5.2.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.2.2...5.2.3) (2022-05-13)


### Bug Fixes

* **ukfis:** Update UKFIS abbreviations ([f70d498](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f70d4985648d325696a41f39ea4acc01cf90ed28))

## [5.2.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.2.1...5.2.2) (2022-05-12)


### Bug Fixes

* **squawk:** treat msfs 1234 default squawk as unassigned ([93182c6](https://github.com/VATSIM-UK/uk-controller-plugin/commit/93182c6098c14583f3abf84251037801ef5cb420))

## [5.2.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.2.0...5.2.1) (2022-04-17)


### Bug Fixes

* **oceanic:** Use new Nattrak API endpoint ([717ad68](https://github.com/VATSIM-UK/uk-controller-plugin/commit/717ad68b7832ae74d74ea9b4146162aca9530f81)), closes [#452](https://github.com/VATSIM-UK/uk-controller-plugin/issues/452)

# [5.2.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.1.1...5.2.0) (2022-04-16)


### Features

* **intentioncode:** E3 intention code ([270d57b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/270d57b3a264013e840ad58b61df6c7d01fa9236)), closes [#458](https://github.com/VATSIM-UK/uk-controller-plugin/issues/458)

## [5.1.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.1.0...5.1.1) (2022-04-05)


### Bug Fixes

* **oceanic:** Crash when updating clearances ([c69ebae](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c69ebae324dbeb6ea5824ab84d90b524b604eb79)), closes [#455](https://github.com/VATSIM-UK/uk-controller-plugin/issues/455)

# [5.1.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/5.0.0...5.1.0) (2022-03-27)


### Features

* **arrival:** Arrival sequencer and spacing aids ([83953e4](https://github.com/VATSIM-UK/uk-controller-plugin/commit/83953e4633a8887f11a1cfe12584b2080826cf75))

# [5.0.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.1...5.0.0) (2022-03-18)


### Bug Fixes

* **loader:** Display more debug information when unable to load core binary ([2e8e87d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/2e8e87da05471527a98d8bb126409771046719eb))
* **loader:** Trigger major version bump ([9fa7052](https://github.com/VATSIM-UK/uk-controller-plugin/commit/9fa7052b0b2993fa95576d8be5ba70998cc374be))
* **updater:** Fix updates not applying properly ([3fa342f](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3fa342f594ab6c935ce5bbb99236c763c23ab0e1))
* **updater:** Make sure new loader works for everyone ([22f1d30](https://github.com/VATSIM-UK/uk-controller-plugin/commit/22f1d308cfec480a09bedc640be9d3a4edc24ec6))


### BREAKING CHANGES

* **loader:** Members may need to download a new version of the loader
to get things to work.

# [5.0.0-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.2-beta.3...5.0.0-beta.1) (2022-03-17)


### Bug Fixes

* **loader:** Trigger major version bump ([9fa7052](https://github.com/VATSIM-UK/uk-controller-plugin/commit/9fa7052b0b2993fa95576d8be5ba70998cc374be))


### BREAKING CHANGES

* **loader:** Members may need to download a new version of the loader
to get things to work.

## [4.4.2-beta.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.2-beta.2...4.4.2-beta.3) (2022-03-17)


### Bug Fixes

* **updater:** Make sure new loader works for everyone ([22f1d30](https://github.com/VATSIM-UK/uk-controller-plugin/commit/22f1d308cfec480a09bedc640be9d3a4edc24ec6))

## [4.4.2-beta.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.2-beta.1...4.4.2-beta.2) (2022-03-16)


### Bug Fixes

* **loader:** Display more debug information when unable to load core binary ([2e8e87d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/2e8e87da05471527a98d8bb126409771046719eb))

## [4.4.2-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.1...4.4.2-beta.1) (2022-03-15)


### Bug Fixes

* **updater:** Fix updates not applying properly ([3fa342f](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3fa342f594ab6c935ce5bbb99236c763c23ab0e1))

## [4.4.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.0...4.4.1) (2022-03-13)


### Bug Fixes

* **historytrails:** Missing first dot on faded trails ([87521f2](https://github.com/VATSIM-UK/uk-controller-plugin/commit/87521f2d9e2a6250b2cd3917133d2d02963c53ea))
* **historytrails:** Missing first dot on faded trails ([fa5d517](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fa5d517bc3e71fb3c4f49e37163f713f240b7c39)), closes [#436](https://github.com/VATSIM-UK/uk-controller-plugin/issues/436)

# [4.4.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.3.0...4.4.0) (2022-03-06)


### Bug Fixes

* **async:** Properly shutdown async tasks on unload ([d58c32b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d58c32b38d0e137b62ff4687bdeb83c3fb3c624f))
* **graphics:** Crash on shutdown ([d0ddd23](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d0ddd2384274297ccb21779a48cb53a73a7654a0))
* **hold:** Hold display breaking when aircraft not in proximity ([d51fc06](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d51fc063532b0cf2b1f9d39402b1ea904cafdb0f))
* **updater:** Updater not respecting selected release channel ([3b5865d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3b5865ded2428e14b9b25c55e222a1e14fb02698))


### Features

* **holds:** Hold timer is triggered by entry time ([f6925aa](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f6925aa2c68744e43db6e4c92f9a2718c26882a9)), closes [#416](https://github.com/VATSIM-UK/uk-controller-plugin/issues/416)

# [4.4.0-beta.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.0-beta.2...4.4.0-beta.3) (2022-03-06)


### Bug Fixes

* **async:** Properly shutdown async tasks on unload ([d58c32b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d58c32b38d0e137b62ff4687bdeb83c3fb3c624f))
* **graphics:** Crash on shutdown ([d0ddd23](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d0ddd2384274297ccb21779a48cb53a73a7654a0))

# [4.4.0-beta.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.4.0-beta.1...4.4.0-beta.2) (2022-03-05)


### Bug Fixes

* **hold:** Hold display breaking when aircraft not in proximity ([d51fc06](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d51fc063532b0cf2b1f9d39402b1ea904cafdb0f))
* **updater:** Updater not respecting selected release channel ([3b5865d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/3b5865ded2428e14b9b25c55e222a1e14fb02698))

# [4.4.0-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.3.0...4.4.0-beta.1) (2022-03-05)


### Features

* **holds:** Hold timer is triggered by entry time ([f6925aa](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f6925aa2c68744e43db6e4c92f9a2718c26882a9)), closes [#416](https://github.com/VATSIM-UK/uk-controller-plugin/issues/416)

# [4.3.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.2.2...4.3.0) (2022-03-02)


### Features

* **intentioncode:** Dont cancel intention codes after leaving the FIR ([#437](https://github.com/VATSIM-UK/uk-controller-plugin/issues/437)) ([fed8a43](https://github.com/VATSIM-UK/uk-controller-plugin/commit/fed8a438ccdccb0523b87ac2b3a10a41e17c394e))
* **wake:** Arrival interval calculations ([#438](https://github.com/VATSIM-UK/uk-controller-plugin/issues/438)) ([45e97a3](https://github.com/VATSIM-UK/uk-controller-plugin/commit/45e97a39611de23777f9b8a86b0927d47c542a97))

## [4.2.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.2.1...4.2.2) (2022-02-14)


### Bug Fixes

* **fonts:** Provide fallback font if EuroScope is not installed ([#434](https://github.com/VATSIM-UK/uk-controller-plugin/issues/434)) ([ce434f3](https://github.com/VATSIM-UK/uk-controller-plugin/commit/ce434f363ab4e2e4747420ab1145ddf2533ac0d0)), closes [#433](https://github.com/VATSIM-UK/uk-controller-plugin/issues/433)

## [4.2.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.2.0...4.2.1) (2022-02-12)
* **initialaltitude:** Dont allow initial altitude recycle on aircraft out of range ([#430](https://github.com/VATSIM-UK/uk-controller-plugin/issues/430)) ([cd3fd0e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/cd3fd0e553626d256863377d1cbe30c6b521261a))
* **initialheading:** Dont allow initial heading recycle on aircraft out of range ([#431](https://github.com/VATSIM-UK/uk-controller-plugin/issues/431)) ([e4fd211](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e4fd211bfde01b0829deaf58a962f1c043654faf)), closes [#429](https://github.com/VATSIM-UK/uk-controller-plugin/issues/429)

# [5.0.0-alpha.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.2.0...5.0.0-alpha.1) (2022-02-12)

### Features

* **holds**: Hold timer by entry time rather than assigned time
* 
# [4.2.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.1.1...4.2.0) (2022-02-10)


### Features

* **pressure:** Make pressure monitor message more brief ([#424](https://github.com/VATSIM-UK/uk-controller-plugin/issues/424)) ([058be53](https://github.com/VATSIM-UK/uk-controller-plugin/commit/058be53e6a40d25d4dd9c5fd490ca24a9661252d)), closes [#423](https://github.com/VATSIM-UK/uk-controller-plugin/issues/423)
* **wake:** Add not applicable to wake calculator display ([#426](https://github.com/VATSIM-UK/uk-controller-plugin/issues/426)) ([6ec1b87](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6ec1b87464db4f5c513048347cf8034c35c37725)), closes [#425](https://github.com/VATSIM-UK/uk-controller-plugin/issues/425)

## [4.1.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.1.0...4.1.1) (2022-02-09)


### Bug Fixes

* **intention:** add golum exit point for copenhagen ([#422](https://github.com/VATSIM-UK/uk-controller-plugin/issues/422)) ([b032890](https://github.com/VATSIM-UK/uk-controller-plugin/commit/b03289000b36586b4e65a05865f9657187c2f6eb)), closes [#421](https://github.com/VATSIM-UK/uk-controller-plugin/issues/421)

# [4.1.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0...4.1.0) (2022-02-05)


### Features

* **wake:** Departure wake interval calculator ([#417](https://github.com/VATSIM-UK/uk-controller-plugin/issues/417)) ([92c6737](https://github.com/VATSIM-UK/uk-controller-plugin/commit/92c6737a17fbafed3dd2de07479889dd58c9fc37))

# [4.0.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.14.0...4.0.0) (2022-01-23)


* 4.0.0 release (#407) ([3503511](https://github.com/VATSIM-UK/uk-controller-plugin/commit/35035110b1ed6313b9c892088d4e12c24987266b)), closes [#407](https://github.com/VATSIM-UK/uk-controller-plugin/issues/407) [#384](https://github.com/VATSIM-UK/uk-controller-plugin/issues/384) [#384](https://github.com/VATSIM-UK/uk-controller-plugin/issues/384) [#386](https://github.com/VATSIM-UK/uk-controller-plugin/issues/386) [#386](https://github.com/VATSIM-UK/uk-controller-plugin/issues/386) [#388](https://github.com/VATSIM-UK/uk-controller-plugin/issues/388) [#388](https://github.com/VATSIM-UK/uk-controller-plugin/issues/388) [#395](https://github.com/VATSIM-UK/uk-controller-plugin/issues/395) [#399](https://github.com/VATSIM-UK/uk-controller-plugin/issues/399) [#397](https://github.com/VATSIM-UK/uk-controller-plugin/issues/397) [#406](https://github.com/VATSIM-UK/uk-controller-plugin/issues/406) [#412](https://github.com/VATSIM-UK/uk-controller-plugin/issues/412) [#409](https://github.com/VATSIM-UK/uk-controller-plugin/issues/409) [#404](https://github.com/VATSIM-UK/uk-controller-plugin/issues/404) [#402](https://github.com/VATSIM-UK/uk-controller-plugin/issues/402)


### BREAKING CHANGES

* The airfield selector for Missed Approach configuration now acts as an override
to the service provision options, rather than a filter for which airfields to use.

* Semantic release for beta

* Remove modifier

* Dump semantic release

* Branch detect

* chore(release): 4.0.0-beta.1 [skip ci]

# [4.0.0-beta.7](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.6...4.0.0-beta.7) (2022-01-23)

### Features

* **holds:** Hold Manager UX Improvements ([#404](https://github.com/VATSIM-UK/uk-controller-plugin/issues/404)) ([c0c999b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c0c999bcc9bd2a4014c1afcbb0ad3c0d1c6ec113))
* **sid:** Allow duplicate SID identifiers when doing initial altitudes ([#402](https://github.com/VATSIM-UK/uk-controller-plugin/issues/402)) ([6aeacfc](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6aeacfcaaad773ef37faa9477586411cc3f785b9))

# [3.14.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.4...3.14.0) (2022-01-21)


### Features

* **holds:** Hold Manager UX Improvements ([#404](https://github.com/VATSIM-UK/uk-controller-plugin/issues/404)) ([c0c999b](https://github.com/VATSIM-UK/uk-controller-plugin/commit/c0c999bcc9bd2a4014c1afcbb0ad3c0d1c6ec113))
* **sid:** Allow duplicate SID identifiers when doing initial altitudes ([#402](https://github.com/VATSIM-UK/uk-controller-plugin/issues/402)) ([6aeacfc](https://github.com/VATSIM-UK/uk-controller-plugin/commit/6aeacfcaaad773ef37faa9477586411cc3f785b9))

# [4.0.0-beta.6](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.5...4.0.0-beta.6) (2022-01-20)


### Bug Fixes

* **handoffs:** Handoffs not repopulating when controllers log on ([#413](https://github.com/VATSIM-UK/uk-controller-plugin/issues/413)) ([d1d872e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d1d872e9fec27a166057a4832493d33d5436a420)), closes [#412](https://github.com/VATSIM-UK/uk-controller-plugin/issues/412)
* **pressuremonitor:** Trigger pressure monitor updates correctly ([#411](https://github.com/VATSIM-UK/uk-controller-plugin/issues/411)) ([287d395](https://github.com/VATSIM-UK/uk-controller-plugin/commit/287d395fbb588e7809492fd9b83c1dca5b1856ea)), closes [#409](https://github.com/VATSIM-UK/uk-controller-plugin/issues/409)

## [3.13.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.3...3.13.4) (2022-01-20)

### Bug Fixes

* **handoffs:** Handoffs not repopulating when controllers log on ([#413](https://github.com/VATSIM-UK/uk-controller-plugin/issues/413)) ([d1d872e](https://github.com/VATSIM-UK/uk-controller-plugin/commit/d1d872e9fec27a166057a4832493d33d5436a420)), closes [#412](https://github.com/VATSIM-UK/uk-controller-plugin/issues/412)
* **pressuremonitor:** Trigger pressure monitor updates correctly ([#411](https://github.com/VATSIM-UK/uk-controller-plugin/issues/411)) ([287d395](https://github.com/VATSIM-UK/uk-controller-plugin/commit/287d395fbb588e7809492fd9b83c1dca5b1856ea)), closes [#409](https://github.com/VATSIM-UK/uk-controller-plugin/issues/409)


# [4.0.0-beta.5](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.4...4.0.0-beta.5) (2022-01-17)

### Bug Fixes
* **intentioncode:** add GOREV as exit fix for K2 intention code ([#406](https://github.com/VATSIM-UK/uk-controller-plugin/issues/406)) ([4135248](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4135248ffd6beb25c107477beee36c7ac72502d7))

## [3.13.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.2...3.13.3) (2022-01-16)

### Bug Fixes

* **intentioncode:** add GOREV as exit fix for K2 intention code ([#406](https://github.com/VATSIM-UK/uk-controller-plugin/issues/406)) ([4135248](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4135248ffd6beb25c107477beee36c7ac72502d7))

# [4.0.0-beta.4](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.3...4.0.0-beta.4) (2022-01-11)

## [3.13.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.1...3.13.2) (2022-01-11)


### Bug Fixes

* **metars:** metars not updating when they should ([#396](https://github.com/VATSIM-UK/uk-controller-plugin/issues/396)) ([4048974](https://github.com/VATSIM-UK/uk-controller-plugin/commit/40489741a88a02ebaf34707cdebf85efd038c426)), closes [#395](https://github.com/VATSIM-UK/uk-controller-plugin/issues/395)
* **push-events:** data not synchronising on second instance startup ([#400](https://github.com/VATSIM-UK/uk-controller-plugin/issues/400)) ([7953627](https://github.com/VATSIM-UK/uk-controller-plugin/commit/7953627c147902e7cf3d38e475b81e075b44100c)), closes [#399](https://github.com/VATSIM-UK/uk-controller-plugin/issues/399)
* **push-events:** Push events not proxying to second EuroScope instance ([#398](https://github.com/VATSIM-UK/uk-controller-plugin/issues/398)) ([e1920fd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e1920fdf2421932b216df43445c713cfc8cc33f9)), closes [#397](https://github.com/VATSIM-UK/uk-controller-plugin/issues/397)


### Bug Fixes

* **metars:** metars not updating when they should ([#396](https://github.com/VATSIM-UK/uk-controller-plugin/issues/396)) ([4048974](https://github.com/VATSIM-UK/uk-controller-plugin/commit/40489741a88a02ebaf34707cdebf85efd038c426)), closes [#395](https://github.com/VATSIM-UK/uk-controller-plugin/issues/395)
* **push-events:** data not synchronising on second instance startup ([#400](https://github.com/VATSIM-UK/uk-controller-plugin/issues/400)) ([7953627](https://github.com/VATSIM-UK/uk-controller-plugin/commit/7953627c147902e7cf3d38e475b81e075b44100c)), closes [#399](https://github.com/VATSIM-UK/uk-controller-plugin/issues/399)
* **push-events:** Push events not proxying to second EuroScope instance ([#398](https://github.com/VATSIM-UK/uk-controller-plugin/issues/398)) ([e1920fd](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e1920fdf2421932b216df43445c713cfc8cc33f9)), closes [#397](https://github.com/VATSIM-UK/uk-controller-plugin/issues/397)

# [4.0.0-beta.3](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.2...4.0.0-beta.3) (2022-01-08)


### Bug Fixes

* **updater:** Handle empty binaries during updates ([#388](https://github.com/VATSIM-UK/uk-controller-plugin/issues/388)) ([dc988b0](https://github.com/VATSIM-UK/uk-controller-plugin/commit/dc988b07292838cd16f127f07da5d020c514aaee))

# [4.0.0-beta.2](https://github.com/VATSIM-UK/uk-controller-plugin/compare/4.0.0-beta.1...4.0.0-beta.2) (2022-01-05)


### Features

* **departure:** Combine pending prenotes and departure releases lists into one ([91c0614](https://github.com/VATSIM-UK/uk-controller-plugin/commit/91c0614982e061a29e18db23e2d9c70c86449fcb))
* **departure:** Combine pending prenotes and releases into one list ([#386](https://github.com/VATSIM-UK/uk-controller-plugin/issues/386)) ([531b687](https://github.com/VATSIM-UK/uk-controller-plugin/commit/531b6876f94a2e6284279efd4ce10c88f2bd92e3))


### BREAKING CHANGES

* **departure:** The old lists are replaced by a single list. New ASR settings
are also used for this list.

# [4.0.0-beta.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.1...4.0.0-beta.1) (2022-01-05)


### Features

* **missedapproach:** Allow airfields to always alert regardless of service provision ([#384](https://github.com/VATSIM-UK/uk-controller-plugin/issues/384)) ([5fe9fd7](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5fe9fd797bfef1bea9fc47e1385ef999e8c6592b))
* **misssedapproach:** Allow airfields to always alert regardless of service provision ([4398182](https://github.com/VATSIM-UK/uk-controller-plugin/commit/4398182e685fc9f1c7ef9236aaeb6dc494aa28aa))


### BREAKING CHANGES

* **misssedapproach:** The airfield selector for Missed Approach configuration now acts as an override
to the service provision options, rather than a filter for which airfields to use.

## [3.13.1](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.13.0...3.13.1) (2022-01-04)


### Bug Fixes

* **intentioncode:** euroscope freezing when aircraft cleared beyond exit point ([#391](https://github.com/VATSIM-UK/uk-controller-plugin/issues/391)) ([5b17241](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5b17241abb7098e53b742b5a359b24090762fb45))

# [3.13.0](https://github.com/VATSIM-UK/uk-controller-plugin/compare/3.12.0...3.13.0) (2022-01-02)


### Bug Fixes

* **srd:** only prepopulate destination airfield into SRD search for EG and EI ([#381](https://github.com/VATSIM-UK/uk-controller-plugin/issues/381)) ([e2f3492](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e2f3492e429c78840148865127245da300431777)), closes [#380](https://github.com/VATSIM-UK/uk-controller-plugin/issues/380)


### Features

* **metar:** Add QFE to pressure monitor messages, QFE query command ([#376](https://github.com/VATSIM-UK/uk-controller-plugin/issues/376)) ([5d32b92](https://github.com/VATSIM-UK/uk-controller-plugin/commit/5d32b925b807409b78e99dd7491f791458fdb436))
* **release:** Allow the plugin to work on multiple update channels ([#387](https://github.com/VATSIM-UK/uk-controller-plugin/issues/387)) ([e2487ec](https://github.com/VATSIM-UK/uk-controller-plugin/commit/e2487ecdb32e57d73aa0aef5a952b16f3f13d3cf))
* **releases:** Allow remarks when approving and rejecting departure releases ([#389](https://github.com/VATSIM-UK/uk-controller-plugin/issues/389)) ([f039d9d](https://github.com/VATSIM-UK/uk-controller-plugin/commit/f039d9d7fc2cbd9d7967f89b40075c04c0825080))
* **srd:** Prepopulate FIR exits in the SRD dialog ([#382](https://github.com/VATSIM-UK/uk-controller-plugin/issues/382)) ([a083cba](https://github.com/VATSIM-UK/uk-controller-plugin/commit/a083cba09a45293401605102cb664480c6cc3357)), closes [#380](https://github.com/VATSIM-UK/uk-controller-plugin/issues/380)

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
