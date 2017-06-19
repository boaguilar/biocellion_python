#!/bin/bash
model_name=boundary-test
model_name=ode
model_xml="examples/${model_name}.xml"
model_dir="../model-auto-${model_name}"
./model_xml_to_cpp.py -m "${model_xml}" -o "${model_dir}"
if [ $? -eq 0 ]; then
    pushd "${model_dir}"
    make -j run
    popd
fi
