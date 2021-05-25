#!/bin/bash

################################################################################
echo
echo "running collate_one_stint_across_series.sh"
echo "---------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 3 )); then
  echo "USAGE: [bucket] [endeavor] [stint]"
  exit 1
fi

bucket="$1"
echo "bucket ${bucket}"
shift

endeavor="$1"
echo "endeavor ${endeavor}"
shift

stint="$1"
echo "stint ${stint}"
shift

################################################################################
echo
echo "other initialization"
echo "--------------------"
################################################################################

[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# adapted from https://stackoverflow.com/a/24114056
script_dir="$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")"
echo "script_dir ${script_dir}"

################################################################################
echo
echo "do collation"
echo "------------"
################################################################################

"${script_dir}/collate_helpers/collate_control_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_control_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_evolve_config_records_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_evolve_config_records_series.sh failed"

"${script_dir}/collate_helpers/collate_evolve_statistics_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_evolve_statistics_series.sh failed"

"${script_dir}/collate_helpers/collate_genome_statistics_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_genome_statistics_series.sh failed"

"${script_dir}/collate_helpers/collate_lowestroot_immediatepredecessor_battle_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_lowestroot_immediatepredecessor_battle_series.sh failed"

"${script_dir}/collate_helpers/collate_monoculture_runninglogs_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_monoculture_runninglogs_series.sh failed"

"${script_dir}/collate_helpers/collate_monoculture_statistics_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_monoculture_statistics_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_deletion_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_deletion_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_deletion_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_deletion_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_insertion_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_insertion_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_insertion_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_insertion_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_point_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_point_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_point_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_point_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_mutant_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutant_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_mutating_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_mutating_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_noncritical_nopout_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_noncritical_nopout_series.sh failed"

"${script_dir}/collate_helpers/collate_noncritical_phenotypeequivalent_nopinterpolation_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_noncritical_phenotypeequivalent_nopinterpolation_competition_series.sh failed"

for state_target in "extrospective" "introspective" "writable"; do
  for perturbation in "exchange" "rotate"; do
    echo "state_target ${state_target}"
    echo "perturbation ${perturbation}"
    "${script_dir}/collate_helpers/collate_perturbation_competition_series.sh" \
      "${bucket}" "${endeavor}" "${stint}" "${state_target}" "${perturbation}"\
      || echo "collate_perturbation_competition_series.sh failed"
  done
done

"${script_dir}/collate_helpers/collate_phenotype_neutral_nopout_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_phenotype_neutral_nopout_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_phenotype_neutral_nopout_phenotype_differentiation_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_phenotype_neutral_nopout_phenotype_differentiation_series.sh failed"

"${script_dir}/collate_helpers/collate_predecessor_battle_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_predecessor_battle_series.sh failed"

for messaging_target in "intra" "inter"; do
  echo "messaging_target ${messaging_target}"
  "${script_dir}/collate_helpers/collate_selfsend_competition_series.sh" \
    "${bucket}" "${endeavor}" "${stint}" "${messaging_target}"\
    || echo "collate_selfsend_competition_series.sh failed"
done

"${script_dir}/collate_helpers/collate_strain_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_strain_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_variant_competition_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_variant_competition_series.sh failed"

"${script_dir}/collate_helpers/collate_wildtype_doubling_time_series.sh" \
  "${bucket}" "${endeavor}" "${stint}" \
  || echo "collate_wildtype_doubling_time_series.sh failed"
