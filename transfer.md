# Debugging Summary: Effect Preset Internal Error

## 1. Problem Description
In non-English locales (e.g., Chinese), selecting certain factory presets for effects like **Reverb**, **Limiter**, and **Compressor** triggered an "Internal Error" (內部錯誤) dialog.

**Root Cause:**
The `GetFactoryPresets()` method in these effects was returning **translated strings** as the preset identifiers. When the `EffectPresetsProvider::applyPreset` method attempted to look up the selected preset using the provided ID, the lookup failed because it was comparing a translated string (e.g., "現代") against a list that the system expected to contain stable, untranslated IDs (e.g., "Modern"). 

When the factory lookup failed, the system attempted to load the preset as a **user preset**. Since the factory preset ID is not a valid user preset, this second attempt also failed, returning `Err::InternalError`.

## 2. Solution
The fix involves a two-part approach to decouple the **Internal ID** from the **Display Name**:

1.  **Stable IDs**: Update `GetFactoryPresets()` in `ReverbEffect`, `LimiterEffect`, and `CompressorEffect` to return the original, untranslated source strings (msgids) using the `.msgid()` method of `TranslatableString`. This ensures that the ID remains constant across all locales, allowing the lookup to succeed.
2.  **UI Translation**: Modify `EffectPresetsBarModel::reload` to translate these stable IDs back into the user's language specifically for the `name` field used in the UI, while keeping the `id` field as the stable English string.

## 3. Current Progress
- [x] **Diagnosis**: Identified the root cause as a mismatch between translated IDs and stable lookup keys.
- [x] **Fix (Internal Error)**: Modified `ReverbEffect`, `LimiterEffect`, and `CompressorEffect` to return stable IDs. This successfully eliminated the "Internal Error".
- [ ] **Fix (UI Regression)**: The current state has a regression where preset names in the UI are displayed in English. The next step is to implement the translation logic in `EffectPresetsBarModel::reload` to restore the localized display names.

### Key Files Modified/Analyzed:
- `src/effects/builtin_collection/reverb/reverbeffect.cpp`
- `src/effects/builtin_collection/dynamics/limiter/limitereffect.cpp`
- `src/effects/builtin_collection/dynamics/compressor/compressoreffect.cpp`
- `src/effects/effects_base/view/effectpresetsbarmodel.cpp`
- `src/effects/effects_base/internal/effectpresetsprovider.cpp`
- `au3/libraries/au3-strings/TranslatableString.h`
