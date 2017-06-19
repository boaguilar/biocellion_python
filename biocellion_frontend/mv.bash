#!/bin/bash
#./model_xml_to_cpp.py -m examples/Vascularperc30-cgl.xml -o ../../biocellion-1.2-mechanisms/model-auto-vascular
#./model_xml_to_cpp.py -m examples/vascular-small.xml -o ../../biocellion-1.2-mechanisms/model-auto-vascular
#./model_xml_to_cpp.py -m examples/chemotaxis.xml -o ../../biocellion-1.2-mechanisms/model-auto-chemotaxis
#./model_xml_to_cpp.py -m examples/Vascularperc30-cgl.xml -o ../model-auto-bc

model_name=boundary-test
model_name=diffusion-test
model_xml="examples/${model_name}.xml"
model_dir="../model-auto-${model_name}"
./model_xml_to_cpp.py -m "${model_xml}" -o "${model_dir}"
if [ $? -eq 0 ]; then
    pushd "${model_dir}"
    make -j run
    popd
fi
