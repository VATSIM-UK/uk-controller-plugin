# Contributing

If you're reading this, you might be curious about contributing to the project. This is awesome!

Below are a few general guidelines for contributors.

## Expectations

- Expectations of contributors can be found in the [Code of Conduct](CODE_OF_CONDUCT.md) document

# Pull Requests

Pull requests with bugfixes are welcome. If you have an idea for a new feature, open an issue for discussion
with the project maintainers before commencing work - just to make sure we're all on the same page. When opening
a pull request, please do the following:

- The title of the PR should be the headline of what has been changed.
- The body of the PR should give a detailed description of what you're changing.
- **Add your changes to the CHANGELOG.md file** found at [CHANGELOG.md](CHANGELOG.md). This allows us to track what
has changed and also give you credit for your hard work!
- Features should be Pull Requested to the `develop` branch.
- Everything else should be Pull Requested to the `master` branch.
- Be prepared to answer questions about your Pull Request when it is reviewed.

## Commits

- When committing, try to make commits "atomic". That is, each task or fix in whatever you are doing should
become one commit to the repository. This is not only useful for reviewers, but allows you to rollback specific
parts of your code without loosing everything else.
- An in-depth guide to atomic commits may be found [here](https://www.freshconsulting.com/atomic-commits/).

## Coding Standards

- All code written should be covered by automated tests. No exceptions. There is no definitive guide
as to what kind of test you should write, but avoid mocking wherever possible.

## Versioning

This repository follows the [SemVer](https://semver.org/) versioning specification.
