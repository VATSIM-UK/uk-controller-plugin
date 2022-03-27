# UK Controller Plugin Changelog

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
